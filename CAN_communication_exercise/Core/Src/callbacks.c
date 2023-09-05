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

uint8_t data[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	  CAN_RxHeaderTypeDef rxPHead = {0};
	  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxPHead, data);
	  CDC_Transmit_FS(data, 8);
//	  CDC_Transmit_FS(rxPHead.IDE, 4);

}
