/*
 * OBDII.c
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#include "OBDII.h"
#include "flags.h"

uint8_t* getState(CAN_HandleTypeDef *hcan){
	  switch(HAL_CAN_GetState(hcan)){
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

void Auto_Baudrate_Setup(CAN_HandleTypeDef *hcan, uint32_t PRE[]){
  uint8_t i;
  for(i = 0; i < 4; i++){
	  if(IRQRX1 == 0 && IRQRX0 == 0){
		  if(i == 0){
			  MX_CAN1_Init(PRE[0], CAN_MODE_SILENT);
			  HAL_CAN_Start(hcan);
			  HAL_Delay(100);
		  }else{
			  HAL_CAN_Stop(hcan);
			  MX_CAN1_Init(PRE[i], CAN_MODE_SILENT);
			  HAL_CAN_Start(hcan);
			  HAL_Delay(100);
		  }
	  }else{
		  break;
	  }
  }

  if(IRQRX1 >= 1 || IRQRX0 >= 1){
	  char str[42];
	  sprintf(str, "BAUDRATE DETECTED WITH PRESCALER OF %ld\n", PRE[i-1]);
	  HAL_CAN_Stop(hcan);
	  hcan->Init.Mode = CAN_MODE_NORMAL;
	  HAL_CAN_Init(hcan);
	  CAN1_Filter_Config(hcan);
	  HAL_CAN_Start(hcan);
  }

  if(IRQRX1 == 0 && IRQRX0 == 0){
//	  CDC_Transmit_FS((uint8_t*)"ERROR DETECTING BAUDRATE", 24);
	  Error_Handler();
  }
}

uint32_t Capture_PID(CAN_HandleTypeDef *hcan, Parameters* PID){
	  PID->LastVal = 0;

	  if(IRQTX == 1){
		  Capture_PID_(hcan, PID->PID);
		  IRQTX = 0;
		  TSLR = HAL_GetTick();
	  }

	  while(IRQTX != 1){
		  if(HAL_GetTick() - TSLR > 5000){
		  		  //**Retransmit request in case there was no response**
		  		  Capture_PID_(hcan, PID->PID);
		  		  TSLR = HAL_GetTick();
		  }
	  }

	  for(uint8_t i = 3; i < LPLD[0]; i++){
		  PID->LastVal = (PID->LastVal<<8) + LPLD[i];
	  }

	  PID->LastVal = (PID->LastVal * PID->Scale) + PID->Offset;

	  return PID->LastVal;
}

//	**Helper**
void Capture_PID_(CAN_HandleTypeDef *hcan, uint8_t PID){
	  uint32_t mailbox;
	  CAN_TxHeaderTypeDef pHead;
	  pHead.StdId = 0x7DF;
	  pHead.IDE = CAN_ID_STD;
	  pHead.RTR = CAN_RTR_DATA;
	  pHead.DLC = 8;

	  uint8_t data[] = {0x02, 0x01, PID, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

	  HAL_CAN_AddTxMessage(hcan, &pHead, data, &mailbox);

}

void CAN1_Filter_Config(CAN_HandleTypeDef *hcan){
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

  if (HAL_CAN_ConfigFilter(hcan, &filters) != HAL_OK)
  {
    Error_Handler();
  }

}

void MX_CAN1_Init(uint32_t Prescaler, uint32_t Mode){

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

  CAN_FilterTypeDef filters = {0};
  filters.FilterActivation = 1;

  HAL_CAN_ConfigFilter(&hcan1, &filters);
  /* USER CODE END CAN1_Init 2 */
}
