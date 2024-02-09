/*
 * LoRa.c
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#include "LoRa.h"
#include "main.h"

uint8_t joined = 0;

uint8_t E5RxBuff[E5RxBuffSize];

//  **Bypass bandwith limitations**
static void Bypass_DCR(UART_HandleTypeDef *huart){
	HAL_Delay(50);
	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+DCS=0\r\n", strlen("AT+DCS=0\n"), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+DR=2\r\n", strlen("AT+DR=2\n"), 1000);
	HAL_Delay(50);
}

void AT_Send(UART_HandleTypeDef *huart, uint8_t* data, uint8_t Chnl){
	uint8_t msg[64] = {0};

	sprintf((char*)msg, "AT+SEND=%d:0:%s\r\n", GPS_PORT,  (char*)data);
//	sprintf((char*)msg, "AT+SEND=%d:1:FFFFFFFF\r\n", GPS_PORT);
	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, msg, strlen((char*)msg), 1000);

}

void AT_Join(UART_HandleTypeDef *huart){

	Bypass_DCR(huart);

	HAL_UARTEx_ReceiveToIdle_DMA(huart, E5RxBuff, E5RxBuffSize);
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+JOIN=1\r\n", 11, 1000);

	uint32_t time = HAL_GetTick();

	while(joined == 0 || joined == 2){
	  if(HAL_GetTick() > time + 10000){
		  HAL_UART_Transmit(huart, (uint8_t*)"AT+JOIN=1\r\n", 11, 1000);
		  time = HAL_GetTick();
	  }

	  if(joined == 2){
		  HAL_UART_Transmit(huart, (uint8_t*)"AT+JOIN=1\r\n", 11, 1000);
		  time = HAL_GetTick();
		  joined = 0;
	  }
//		  waiting for data
	}

}


//OLD
void LoRa_Init(UART_HandleTypeDef *huart){
	uint8_t msg[64] = {0};

	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+UART=TIMEOUT,300\r\n", 21, 500);

	HAL_Delay(200);

	sprintf((char*)msg, "AT+ID=DevEui,\"%s\"\r\n", LORAWAN_DEVICE_EUI);
	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, msg, strlen((char*)msg), 1000);

	HAL_Delay(200);

	sprintf((char*)msg, "AT+ID=AppEui,\"%s\"\r\n", LORAWAN_JOIN_EUI);
	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, msg, strlen((char*)msg), 1000);

	HAL_Delay(200);

	sprintf((char*)msg, "AT+KEY=APPKEY,\"%s\"\r\n", LORAWAN_APP_KEY);
	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, msg, strlen((char*)msg), 1000);

	HAL_Delay(200);

	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+ID\r\n", strlen((char*)"AT+ID\r\n"), 1000);

	HAL_Delay(200);

	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+MODE=LWOTAA\r\n", strlen((char*)"AT+MODE=LWOTAA\r\n"), 1000);

	HAL_Delay(200);

	HAL_UART_Transmit(huart, (uint8_t*)PREAMBLE, strlen((char*)PREAMBLE), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+LOWPOWER=AUTOON\r\n", strlen((char*)"AT+LOWPOWER=AUTOON\r\n"), 1000);
}
