
#include "StationTelemetry.h"

/*---BEGIN---------My externed variable------------*/
extern UART_HandleTypeDef huart2;
/*---END-----------My externed variable------------*/

/*---BEGIN---------My private variable------------*/
Variable_HandleTypeDef data;
uint16_t Written_Bytes; /* is for save number of total converted buffer's characters*/
char tx_buffer[250];
/*---END-----------My private variable------------*/


void SATELLITE_fill_array(){

	/*! Write 0 into the transmit buffer before uploading */
	for(int j = 0 ; j < 250 ; j++){
		tx_buffer[j] = 0;
	}

	/*! Write all telemetry datas into the tx_buffer to send to the station pc */
	Written_Bytes = sprintf(tx_buffer, "<%d>,<%d>,<%u>,<%u/%u/%u,%u/%u/%u>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,<%c/%c/%c/%c>,<%.2f>,<%d>,<%.2f>,<%.2f>,<%.2f>,<%.2f>,\n",
																																																	 data.packetnum,
																																																	 data.satStatus,
																																																	 data.errorCode,
																																																	 data.day,
																																																	 data.mounth,
																																																	 data.year,
																																																	 data.hour,
																																																	 data.minute,
																																																	 data.second,
																																																	 data.press1,
																																																	 data.press2,
																																																	 data.vertHeight1,
																																																	 data.vertHeight2,
																																																	 data.diffVertHeight,
																																																	 data.vertSpeed,
																																																	 data.temp,
																																																	 data.voltBat,
																																																	 data.gps1_Latitude,
																																																	 data.gps1_Longtitude,
																																																	 data.gps1_Altitude,
																																																	 data.pitch,
																																																	 data.roll,
																																																	 data.yaw,
																																																	 data.rhrh[0],
																																																	 data.rhrh[1],
																																																	 data.rhrh[2],
																																																	 data.rhrh[3],
																																																	 data.IOTdata,
																																																	 data.teamNo,
																																																	 data.gps2_Latitude,
																																																	 data.gps2_Longtitude,
																																																	 data.temp2,
																																																	 data.voltBat2);


}

void SATELLITE_set_first_values(){

	data.packetnum		= 0; 		// increase +1 at end of the period
	data.satStatus		= 0;		// we will determine according to the mission part
	data.errorCode		= 4;		// we will determine our scenario

	data.second			= 12;		// Change each end of period, increase as +1
	data.minute			= 22;		// if second is 60, increase minute +1
	data.hour			= 17;		// if minute is 60, increase hours +1

	data.day			= 28;		//fix
	data.mounth			= 6;		//fix
	data.year			= 2024; 	//fix

	data.press1			= 101325.64;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
	data.press2			= 101325.88;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
	data.vertHeight1	= 0;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 8m/s
	data.vertHeight2	= 0;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 6m/s
	data.diffVertHeight = 0;
	data.vertSpeed		= 0;

	data.temp			= 24.57;	   // reduce as 0.01 celcius each period
	data.voltBat		= 8.3;		   // reduce as 0.01 volt each period

	data.temp2			= 24.57;	   // reduce as 0.01 celcius each period
	data.voltBat2		= 8.3;		   // reduce as 0.01 volt each period

	data.gps1_Latitude 	= 3402.43;	   // change 0.3 , each 0.1 is equal 8meter
	data.gps1_Longtitude= 3819.59;	   // change 0.3 , each 0.1 is equal 8meter
	data.gps1_Altitude	= 1000;		   // measure altitude from sealevel, increase when you're climbing up with 50m , and after that reduce 12m/s, and 8m/s

	data.gps2_Latitude 	= 3102.43;	   // change 0.3 , each 0.1 is equal 8meter
	data.gps2_Longtitude= 3519.59;	   // change 0.3 , each 0.1 is equal 8meter
	data.gps2_Altitude	= 1000;		   // measure altitude from sealevel, increase when you're climbing up with 50m , and after that reduce 12m/s, and 8m/s

	data.pitch			= 0.5;		   //
	data.roll			= 2.3125;	   //
	data.yaw			= 359.625;	   //fixed

	data.rhrh[0] =  '?';		   			   //fixed
	data.rhrh[1] =  '?';		  			   //fixed
	data.rhrh[2] =  '?';		   			   //fixed
	data.rhrh[3] =  '?';	   			   	   //fixed

	data.IOTdata		= 23.57;	   //reduce as 0.01 celcius each period
	data.teamNo			= 270061;	   //fixed

	SATELLITE_fill_array();

	if(data.PermissionFlag == Permission_On){
		HAL_UART_Transmit(&huart2, tx_buffer, Written_Bytes,1000);
	}
	data.PermissionFlag = Permission_Off;
}


// 0 ready for flight , 1 flying , 2 falling down , 3 separating , 4 payload falling down , 5 saving
//process will be going on for about 25 second
void SATELLITE_ready_for_flight__to__apogie(){

	//it's flying now
	for(int i = 0 ; i < 25 ; i++){

		data.packetnum		+= 1; 		// increase +1 at end of the period

		if(i == 2){
			data.satStatus = 1;
		}
		else if(i == 24){
			data.satStatus = 2;
		}

		data.errorCode		= 2;		// we will determine our scenario

		data.second			+= 1;		// Change each end of period, increase as +1
		data.minute			= 22;		// if second is 60, increase minute +1


		data.press1			-= 12;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
		data.press2			-= 12;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
		data.vertHeight1	+= 50.22;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 8m/s
		data.vertHeight2	+= 50.22;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 6m/s
		data.diffVertHeight = 0;
		data.vertSpeed		+= 10.34;

		data.temp			-= 0.05;	   // reduce as 0.01 celcius each period
		data.voltBat		-= 0.01;		   // reduce as 0.01 volt each period

		data.temp2			-= 0.05;	   // reduce as 0.01 celcius each period
		data.voltBat2		-= 0.01;		   // reduce as 0.01 volt each period

		data.gps1_Latitude 	+= 0.3 ;	   // change 0.3 , each 0.1 is equal 8meter
		data.gps1_Longtitude +=0.3;	   // change 0.3 , each 0.1 is equal 8meter
		data.gps1_Altitude	+= 50;		   // measure altitude from sealevel, increase when you're climbing up with 50m , and after that reduce 12m/s, and 8m/s

		data.gps2_Latitude += 0.3;	   // change 0.3 , each 0.1 is equal 8meter
		data.gps2_Longtitude += 0.3;	// change 0.3 , each 0.1 is equal 8meter
		data.gps2_Altitude += 50;// measure altitude from sealevel, increase when you're climbing up with 50m , and after that reduce 12m/s, and 8m/s


		data.pitch			+= 2.5;		   //
		data.roll			+= 2.3125;	   //
		data.yaw			= 359.625;	   //fixed

		data.IOTdata		+= 0.1;	   //reduce as 0.01 celcius each period
		data.teamNo			= 270061;	   //fixed

		SATELLITE_fill_array();

		if(data.PermissionFlag == Permission_On){
			HAL_UART_Transmit(&huart2, tx_buffer, Written_Bytes,1000);
		}
		data.PermissionFlag = Permission_Off;
		HAL_Delay(1000);

	}

}


void SATELLITE_apogie__to__ground(){

	for(int i = 0 ; i < 58 ; i++){

		data.packetnum		+= 1; 		// increase +1 at end of the period

		if(i == 10){
			data.satStatus	= 3 ;		// separating
		}
		else if(i == 12){
			data.satStatus	= 4 ;		// payload is falling down
		}
		else if(i == 40){
		data.errorCode		= 5;		// payload is waiting to save
		}


		data.second			+= 1;		// Change each end of period, increase as +1
		if(data.second == 60){
			data.minute		+= 1;
			data.second     = 0;
		}

					if(i <= 39){
						data.press1			+= 12;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
						data.press2			+= 12;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
						data.vertHeight1	-= 50;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 8m/s
						data.vertHeight2	-= 50;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 6m/s
						data.diffVertHeight += 5;
						data.vertSpeed		= 14;

						data.temp			+= 0.05;	   // reduce as 0.01 celcius each period
						data.voltBat		-= 0.01;		   // reduce as 0.01 volt each period

						data.gps1_Latitude 	-= 0.3 ;	   // change 0.3 , each 0.1 is equal 8meter
						data.gps1_Longtitude -=0.3;	   // change 0.3 , each 0.1 is equal 8meter
						data.gps1_Altitude	-= 50;		   // measure altitude from sealevel, increase when you're climbing up with 50m , and after that reduce 12m/s, and 8m/s

						data.gps2_Latitude += 0.3;	// change 0.3 , each 0.1 is equal 8meter
						data.gps2_Longtitude += 0.3;// change 0.3 , each 0.1 is equal 8meter
						data.gps2_Altitude += 50;// measure altitude from sealevel, increase when you're climbing up with 50m , and after that reduce 12m/s, and 8m/s

						data.pitch			-= 2.5;		   //
						data.roll			-= 2.3125;	   //
						data.yaw			= 359.625;	   //fixed

						data.temp2			+= 0.05;	   // reduce as 0.01 celcius each period
						data.voltBat2		-= 0.01;		   // reduce as 0.01 volt each period

						data.IOTdata		-= 0.1;
					}
					else if(i >= 40){
						data.press1			-= 0;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
						data.press2			+= 12;   // reduce 12 pascal at each meter, 375meter is equal to 97000pascal
						data.vertHeight1	+= 0;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 8m/s
						data.vertHeight2	-= 50;		   // increase quickly at the beginning time like 50m/s , after that 12m/s until module is separated and finally 6m/s
						data.diffVertHeight -= 5;
						data.vertSpeed		= 8;
						data.voltBat		-= 0.01;		   // reduce as 0.01 volt each period
						data.voltBat2		-= 0.01;
					}


		SATELLITE_fill_array();

		if(data.PermissionFlag == Permission_On){
			HAL_UART_Transmit(&huart2, tx_buffer, Written_Bytes,1000);
		}
		data.PermissionFlag = Permission_Off;
		HAL_Delay(1000);

	}

}

