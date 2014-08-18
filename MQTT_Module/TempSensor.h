/*
 * TempSensor.h
 *
 * Created: 12.08.2014 13:39:10
 *  Author: gsteindl
 */ 


#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_



#define ONE_WIRE_BUS_PIN 6
#define SIZE_ADDRESS_TABLE 5
#define DEFAULT_RESOLUTION 10

#include "ErrorCodes.h"
#include <inttypes.h>

struct tableEntry_s{
	uint8_t address[8];
	uint8_t tempSensorID;
	float sensorValue;
	uint8_t state;

	} typedef tableEntry_t;

struct tempSensorTable_s{
	uint8_t size;
	tableEntry_t tableEntry[SIZE_ADDRESS_TABLE];
	
	} typedef tempSensorTable_t;


//Interface
void tempSenosrsInit(void);
void tempSenosrsInit(const uint8_t resolution);
void tempSensorsPrintInfo(void);
void tempSensorPrintTable(void);
error_t tempSensorRead(void);
error_t turnAllTempSensorsOff();
error_t turnAllTempSensorsOn();

void testMergeTable();
#endif /* TEMPSENSOR_H_ */