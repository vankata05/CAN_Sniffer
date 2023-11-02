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

#define MSG "PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
#define MSG2 "PMTK458"

void xyz_to_wgs84(double x, double y, double z, double *latitude, double *longitude);
void GNSS_Transmit(UART_HandleTypeDef *huart, uint8_t* msg);
void GNSS_Get_Coords(UART_HandleTypeDef *huart, uint8_t size, uint32_t* lat, uint32_t* lon);
uint8_t CRC_(const uint8_t* str);

#endif /* INC_GNSS_H_ */
