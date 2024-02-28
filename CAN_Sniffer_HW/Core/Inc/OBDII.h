/*
 * OBDII.h
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#ifndef INC_OBDII_H_
#define INC_OBDII_H_

#include "main.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	uint8_t PID;
	uint32_t Scale;
	uint8_t Offset;
	uint8_t LastVal[8];
}Parameters;

extern uint8_t payload[8];
extern uint32_t PRE[];
extern Parameters PIDs[];
extern uint8_t BDTKTD;
extern volatile uint8_t IRQRX0;
extern volatile uint8_t IRQRX1;


uint8_t* getState(CAN_HandleTypeDef *hcan);
void CAN1_Init(uint32_t Prescaler, uint32_t Mode);
void CAN1_Filter_Config(CAN_HandleTypeDef *hcan);
void Capture_PID_(CAN_HandleTypeDef *hcan, uint8_t PID);
uint8_t* Capture_PID(CAN_HandleTypeDef *hcan, Parameters* PID);
void Auto_Baudrate_Setup(CAN_HandleTypeDef *hcan, uint32_t PRE[]);

#endif /* INC_OBDII_H_ */
