/*
 * GNSS.h
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#ifndef INC_GNSS_H_
#define INC_GNSS_H_

#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define MSG "PMTK314,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
#define MSG2 "PMTK458"

#define L96RxBuffSize 64
#define L96MainBuffSize 64

extern uint8_t L96RxBuff[L96RxBuffSize];
extern uint8_t L96MainBuff[L96MainBuffSize];

typedef struct {
	uint32_t lat;
	uint32_t lon;
}GPS;

void xyz_to_wgs84(double x, double y, double z, double *latitude, double *longitude);
void GNSS_Transmit(UART_HandleTypeDef *huart, uint8_t* msg);
void GNSS_Init(UART_HandleTypeDef *huart);
void pharse_GLL(uint8_t* payload);
uint8_t GNSS_Get_Coords(UART_HandleTypeDef *huart);
uint8_t CRC_(const uint8_t* str);

#endif /* INC_GNSS_H_ */
