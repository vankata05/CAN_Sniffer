/*
 * GNSS.c
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#include "GNSS.h"

uint8_t L96RxBuff[L96RxBuffSize];
uint8_t L96MainBuff[L96MainBuffSize];

GPS L96Data = {0, 0};

uint8_t received = 0;

void GNSS_Init(UART_HandleTypeDef *huart){
	GNSS_Transmit(huart, (uint8_t*)"PMTK104");
	HAL_Delay(1000);
	GNSS_Transmit(huart, (uint8_t*)MSG);
	HAL_UARTEx_ReceiveToIdle_DMA( huart, L96MainBuff, L96MainBuffSize);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
}

uint8_t CRC_(const uint8_t* str) {
	uint8_t checksum = 0;

    for (int i = 0; i < strlen((char*)str); i++) {
        checksum ^= (uint8_t)str[i];
    }

    return checksum;
}

void xyz_to_wgs84(double x, double y, double z, double *latitude, double *longitude) {
    double r = sqrt(x*x + y*y + z*z);
    double lon = atan2(y, x);
    double lat = asin(z / r);

    *latitude = lat * 180.0 / M_PI;  // Convert to degrees
    *longitude = lon * 180.0 / M_PI; // Convert to degrees
}


uint8_t GNSS_Get_Coords(UART_HandleTypeDef *huart){

	  GNSS_Transmit(huart, (uint8_t*)MSG2);
	  uint32_t time = HAL_GetTick();
	  while(received == 0){
		  if(HAL_GetTick() > time + 1000){
			  GNSS_Transmit(huart, (uint8_t*)MSG2);
			  time = HAL_GetTick();
		  }
//		  waiting for data
	  }

//	  HAL_UART_Transmit(huart, L96MainBuff, strlen((char*)L96MainBuff), 1000);
//
//	  HAL_Delay(1000);

	  if(strstr((char*) L96MainBuff, "0.0,0.0,") != NULL){
//		  **Test purposes**
//		  strcpy((char*)L96MainBuff, (char*)"$PMTK558,4311951.2,1863684.4,4300899.3,59.5*05\r\n");

		  L96Data.lat = 0;
		  L96Data.lon = 0;
		  memset(L96MainBuff, 0, 64);
		  return 0;
	  }

	  strtok((char*)L96MainBuff, ",");

	  double lat_, lon_;

	  xyz_to_wgs84(atof(strtok(NULL, ",")) , atof(strtok(NULL, ",")), atof(strtok(NULL, ",")), &lat_, &lon_);

	  L96Data.lat = (uint32_t)(lat_*1000000);
	  L96Data.lon = (uint32_t)(lon_*1000000);

	  memset(L96MainBuff, 0, 64);

	  return 1;
}

void GNSS_Transmit(UART_HandleTypeDef *huart, uint8_t* msg){
	  uint8_t req[64] = {0};
	  sprintf((char*)req, "$%s*%X\r\n", msg, CRC_((uint8_t*)msg));
	  HAL_UART_Transmit(huart, req, strlen((char*)req), 1000);
}
