/*
 * LoRa.c
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#include "LoRa.h"

//  **Bypass bandwith limitations**
static void Bypass_DCR(UART_HandleTypeDef *huart){
	HAL_Delay(50);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+DCS=0\n", strlen("AT+DCS=0\n"), 1000);
	HAL_UART_Transmit(huart, (uint8_t*)"AT+DR=2\n", strlen("AT+DR=2\n"), 1000);
	HAL_Delay(50);
}

void AT_Send(UART_HandleTypeDef *huart, uint8_t* data, uint8_t Chnl){

	uint8_t msg[64] = {0};
	sprintf((char*)msg, "AT+SEND=%d:0:%s\n", Chnl, (char*)data);
	HAL_UART_Transmit(huart, msg, strlen((char*)msg), 1000);

}

void AT_Join(UART_HandleTypeDef *huart){
	//Reset teh LoRa E5 module
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);

	HAL_Delay(100);

	//Start JOIN
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

	Bypass_DCR(huart);

	uint8_t msg[64] = {0};
	uint32_t tick = HAL_GetTick();
	HAL_UART_Transmit(huart, (uint8_t*)"AT+JOIN=1\n", 10, 1000);
	while(strstr((char*) msg, "JOINED") == NULL){
		if(HAL_GetTick() - tick >= 10000 || strstr((char*) msg, "FAILED") != NULL){
			tick = HAL_GetTick();
			HAL_UART_Transmit(huart, (uint8_t*)"AT+JOIN=1\n", 10, 1000);
		}
		HAL_UART_Receive_STR(huart, msg, 64, 50);
	}
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}
