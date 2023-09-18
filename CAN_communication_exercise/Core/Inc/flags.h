/*
 * flags.h
 *
 *  Created on: Aug 31, 2023
 *      Author: IVI6SF
 */

#ifndef SRC_FLAGS_H_
#define SRC_FLAGS_H_
#include "stm32f4xx_hal.h"  // Include the appropriate STM32 header

// Global flag variables defined as extern
extern volatile uint8_t IRQRX0;
extern volatile uint8_t IRQRX1;
extern volatile uint8_t IRQTX;
extern volatile uint8_t BDTKTD;
extern volatile uint8_t TSLR;

// Global can1 structure defined as extern
extern CAN_HandleTypeDef hcan1;

#endif /* SRC_FLAGS_H_ */
