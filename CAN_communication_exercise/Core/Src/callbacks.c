/*
 * callbacks.c
 *
 *  Created on: Aug 28, 2023
 *      Author: IVI6SF
 */
#include "usbd_cdc_if.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#include "flags.h"

extern uint8_t* getState();

uint8_t data[8];
char data1[64];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//	if(BDTKTD == 1){

	  CAN_RxHeaderTypeDef rxPHead = {0};
	  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxPHead, data);
	  sprintf(data1, "0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", (unsigned int)rxPHead.StdId, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
//	  HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &rxPHead, data);
//	  if(rxPHead.StdId == 0x7E8){
	  CDC_Transmit_FS((uint8_t*)data1, strlen(data1));
//	  }
//	  CDC_Transmit_FS(data, 8);
//	  CDC_Transmit_FS((uint8_t)rxPHead.StdId, 2);
//	}
}
