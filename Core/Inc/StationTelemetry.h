/*
 * StationTelemetry.h
 *
 *  Created on: Jun 27, 2024
 *      Author: yunus
 */

#ifndef INC_STATIONTELEMETRY_H_
#define INC_STATIONTELEMETRY_H_

#include "main.h"

typedef enum{

 Permission_On = 1,
 Permission_Off

}Permission_StateTypeDef;

typedef struct{

	uint32_t packetnum;

	uint8_t  satStatus;

	uint8_t	 errorCode;

	uint8_t	 second;
	uint8_t  minute;
	uint8_t  hour;
	uint8_t  day;
	uint8_t  mounth;
	uint16_t year;

	float press1;
	float press2;
	float vertHeight1;
	float vertHeight2;
	float diffVertHeight;

	float vertSpeed;
	float temp;
	float voltBat;
	/* For carrier*/
	float temp2;
	float voltBat2;


	float gps1_Latitude;
	float gps1_Longtitude;
	float gps1_Altitude;
	/* For carrier*/
	float gps2_Latitude;
	float gps2_Longtitude;
	float gps2_Altitude;


	float pitch;
	float roll;
	float yaw;

	char rhrh[4];

	float IOTdata;

	uint32_t teamNo;

	uint8_t PermissionFlag;
}Variable_HandleTypeDef;


//process will be going on for about 25 second
void SATELLITE_ready_for_flight__to__apogie();

void SATELLITE_apogie__to__ground();

// sets the variable first data when the system is on first moment
void SATELLITE_set_first_values();

void SATELLITE_fill_array();

#endif /* INC_STATIONTELEMETRY_H_ */
