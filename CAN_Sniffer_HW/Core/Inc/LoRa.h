/*
 * LoRa.h
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#ifndef INC_LORA_H_
#define INC_LORA_H_

#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LORAWAN_DEVICE_EUI	"<!!!REPLACE_WITH_CREDENTIALS!!!>"
#define LORAWAN_JOIN_EUI    "<!!!REPLACE_WITH_CREDENTIALS!!!>"
#define LORAWAN_APP_KEY     "<!!!REPLACE_WITH_CREDENTIALS!!!>"

#define E5RxBuffSize 64

extern uint8_t E5RxBuff[E5RxBuffSize];

//#define PREAMBLE "\xFF\xFF\xFF\xFF"
#define PREAMBLE ""

void AT_Join(UART_HandleTypeDef *huart);
void AT_Send(UART_HandleTypeDef *huart, uint8_t* data, uint8_t Chnl);
void LoRa_Init(UART_HandleTypeDef *huart);

#endif /* INC_LORA_H_ */
