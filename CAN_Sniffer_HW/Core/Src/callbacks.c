#include "main.h"
#include "GNSS.h"
#include "LoRa.h"
#include "OBDII.h"

#include <string.h>
#include <stdlib.h>

extern uint8_t received;

extern uint8_t joined;

extern uint8_t IRQRX;
extern uint8_t BDTKTD;

uint8_t oldPos = 0;
uint8_t newPos = 0;

extern UART_HandleTypeDef huart3;

//uint8_t cnt = 0;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxPHead = {0};
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxPHead, payload);

	HAL_UART_Transmit(&huart3, payload, 8, 1000);

	IRQRX = 1;
	if(BDTKTD == 1){
//	  HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader, uint8_t aData[])
//	  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxPHead, payload);
	  HAL_Delay(1);

	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
//	L96 RX
	if (huart->Instance == USART3)
	{
		oldPos = newPos;

		if (oldPos+Size > L96MainBuffSize)
		{
			uint16_t datatocopy = L96MainBuffSize-oldPos;
			memcpy ((uint8_t *)L96MainBuff+oldPos, L96RxBuff, datatocopy);

			oldPos = 0;
			memcpy ((uint8_t *)L96MainBuff, (uint8_t *)L96RxBuff+datatocopy, (Size-datatocopy));
			newPos = (Size-datatocopy);
		}

		else
		{
			memcpy ((uint8_t *)L96MainBuff+oldPos, L96RxBuff, Size);
			newPos = Size+oldPos;
		}

		if(strstr((char*)L96MainBuff, "$PMTK558,") || strstr((char*)L96MainBuff, "$GNGLL,") != NULL){
			received = 1;
		}

		HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t *) L96RxBuff, L96RxBuffSize);
		__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);

		if(strstr((char*)L96MainBuff, "$GNGLL,") != NULL){
			pharse_GLL(L96MainBuff);
		}

	}

//	Wio-E5
	if (huart->Instance == USART2)
	{
		if(strstr((char*)E5RxBuff, "JOINED") != NULL){
			joined = 1;
		}else{
			HAL_UARTEx_ReceiveToIdle_DMA(huart, E5RxBuff, E5RxBuffSize);
			__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
		}

		if(strstr((char*)E5RxBuff, "FAILED") != NULL)
			joined = 2;

	}
}
