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

void AT_Join(UART_HandleTypeDef *huart);
void AT_Send(UART_HandleTypeDef *huart, uint8_t* data, uint8_t Chnl);

#endif /* INC_LORA_H_ */
