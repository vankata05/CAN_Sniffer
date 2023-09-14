/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
volatile uint8_t IRQRX0;
volatile uint8_t IRQRX1;
volatile uint8_t IRQTX;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(uint32_t Prescaler, uint32_t Mode);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t* getState(){
	  switch(HAL_CAN_GetState(&hcan1)){
	  	  case 0x00U:
	  		  return (uint8_t*)"HAL_CAN_STATE_RESET\n";
	  		  break;
	  	  case 0x01U:
	  		  return (uint8_t*)"HAL_CAN_STATE_READY\n";
	  		  break;
	  	  case 0x02U:
	  		  return (uint8_t*)"HAL_CAN_STATE_LISTENING\n";
	  		  break;
	  	  case 0x03U:
	  		  return (uint8_t*)"HAL_CAN_STATE_SLEEP_PENDING\n";
	  		  break;
	  	  case 0x04U:
	  		  return (uint8_t*)"HAL_CAN_STATE_SLEEP_ACTIVE\n";
	  		  break;
	  	  default:
	  		  return (uint8_t*)"HAL_CAN_STATE_ERROR\n";
	  		  break;
	  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET){
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
  }
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

  uint32_t PRE[4] = {42, 210, 84, 21};

  uint8_t i;
  for(i = 0; i < 5; i++){
	  if(IRQRX1 == 0 && IRQRX0 == 0){
		  if(i == 0){
			  MX_CAN1_Init(PRE[0], CAN_MODE_SILENT);
			  HAL_CAN_Start(&hcan1);
			  HAL_Delay(100);
		  }else{
//			  HAL_GPIO_LockPin(GPIOD, GPIO_PIN_0);
//			  HAL_GPIO_LockPin(GPIOD, GPIO_PIN_1);
			  HAL_CAN_Stop(&hcan1);
//			  HAL_Delay(100);
			  MX_CAN1_Init(PRE[i], CAN_MODE_SILENT);
			  HAL_CAN_Start(&hcan1);
			  HAL_Delay(100);
		  }
	  }else{
		  if(IRQRX1 == 1 || IRQRX0 == 1){
			  char str[42];
			  sprintf(str, "BAUDRATE DETECTED WITH PRESCALER OF %ld\n", PRE[i-1]);
			  CDC_Transmit_FS((uint8_t*)str, strlen(str));
			  HAL_CAN_Stop(&hcan1);
//			  MX_CAN1_Init(PRE[i], CAN_MODE_NORMAL);
			  hcan1.Init.Mode = CAN_MODE_NORMAL;
			  HAL_CAN_Init(&hcan1);
			  HAL_CAN_Start(&hcan1);

		  }
	  }
  }

  if(IRQRX1 == 0 && IRQRX0 == 0){
	  CDC_Transmit_FS((uint8_t*)"ERROR DETECTING BAUDRATE", 24);
	  Error_Handler();
  }

//    **Transmit**
//  **PIDs Supported**
  uint32_t mailbox;
  CAN_TxHeaderTypeDef pHead;
  pHead.StdId = 0x7DF;
  pHead.IDE = CAN_ID_STD;
  pHead.RTR = CAN_RTR_DATA;
  pHead.DLC = 8;

  uint8_t data[] = {0x02, 0x01, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

  //  **PIDs supported(01-20)**
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);
  HAL_Delay(50);

  //  **PIDs supported(21-40)**
  data[2] = 0x20;
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);
  HAL_Delay(50);

  //  **PIDs supported(41-60)**
  data[2] = 0x40;
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);
  HAL_Delay(50);

  //  **PIDs supported(61-80)**
  data[2] = 0x60;
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);
  HAL_Delay(50);

  //  **PIDs supported(81-A0)**
  data[2] = 0x80;
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);
  HAL_Delay(50);

  //  **PIDs supported(A1-C0)**
  data[2] = 0xA0;
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);
  HAL_Delay(50);

  //  **PIDs supported(C1-E0)**
  data[2] = 0xC0;
  HAL_CAN_AddTxMessage(&hcan1, &pHead, data, &mailbox);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */

static void MX_CAN1_Init(uint32_t Prescaler, uint32_t Mode)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = Prescaler;
  hcan1.Init.Mode = Mode;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  HAL_CAN_ActivateNotification(&hcan1, 0xFFFFFFFFU);

//  **Configure filters**
  CAN_FilterTypeDef filters = {0};


  filters.FilterActivation = ENABLE;
  filters.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  filters.FilterBank = 0;
  filters.FilterMode = CAN_FILTERMODE_IDMASK;
  filters.FilterScale = CAN_FILTERSCALE_32BIT;
  filters.FilterIdHigh = 0x7E8 << 5;
  filters.FilterIdLow = 0;
  filters.FilterMaskIdHigh = 0x7fd << 5;
  filters.FilterMaskIdLow = 0;

//  memset(&filters, 0, sizeof(CAN_FilterTypeDef));

  if (HAL_CAN_ConfigFilter(&hcan1, &filters) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END CAN1_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
