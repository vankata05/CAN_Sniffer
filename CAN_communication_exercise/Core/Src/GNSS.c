/*
 * GNSS.c
 *
 *  Created on: Oct 31, 2023
 *      Author: IVI6SF
 */

#include "GNSS.h"

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


void GNSS_Get_Coords(UART_HandleTypeDef *huart, uint8_t size, uint32_t* lat, uint32_t* lon){
	  uint8_t data[64] = {0};

	  GNSS_Transmit(huart, (uint8_t*)MSG2);

	  while(1){
//		  HAL_UART_Receive_STR(huart, data, size, 50);
		  strcpy((char*)data, (char*)"$PMTK558,4311951.2,1863684.4,4300899.3,59.5*05\r\n");
		  if(strstr((char*) data, "$PMTK558") != NULL){
			  break;
		  }else if(strstr((char*) data, "0.0,0.0,") != NULL){
			  *lat = -1;
			  *lon = -1;
			  return;
		  }
	  }

	  strtok((char*)data, ",");

	  double lat_, lon_;

	  xyz_to_wgs84(atof(strtok(NULL, ",")) , atof(strtok(NULL, ",")), atof(strtok(NULL, ",")), &lat_, &lon_);

	  *lat = (uint32_t)(lat_*1000000);
	  *lon = (uint32_t)(lon_*1000000);
}

void GNSS_Transmit(UART_HandleTypeDef *huart, uint8_t* msg){
	  uint8_t req[64] = {0};
	  sprintf((char*)req, "$%s*%X\r\n", msg, CRC_((uint8_t*)msg));
	  HAL_UART_Transmit(huart, req, strlen((char*)req), 1000);
}
