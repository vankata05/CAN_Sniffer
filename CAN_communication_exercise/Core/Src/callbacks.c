/*
 * callbacks.c
 *
 *  Created on: Aug 28, 2023
 *      Author: IVI6SF
 */
#include "usbd_cdc_if.h"
#include "stm32f4xx_hal.h"
#include <string.h>

extern uint8_t* getState();
extern CAN_HandleTypeDef hcan1;
//extern CAN_HandleTypeDef hcan2;

uint8_t data[8];
char data1[30];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	  CAN_RxHeaderTypeDef rxPHead = {0};
	  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxPHead, data);
	  sprintf(data1, "|%X|%X|%X|%X|%X|%X|%X|%X|\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
//	  HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &rxPHead, data);
	  CDC_Transmit_FS((uint8_t*)data1, 30);
//	  CDC_Transmit_FS(rxPHead.IDE, 4);

}
