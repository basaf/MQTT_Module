/*
 * EEPROMSaver.h
 *
 * Created: 12.08.2014 16:19:50
 *  Author: gsteindl
 */ 


#ifndef EEPROMSAVER_H_
#define EEPROMSAVER_H_

#include "ErrorCodes.h"
#include "TempSensor.h"

#define TEMP_SENSOR_TABLE_MEMORY_ADDRESS 0x0A
#define FLAG_REGION 0x00

//Flags
#define TEMP_SENS_TABLE_VALID_FLAG 0x01

error_t writeBlockToEEPROM(void *block, unsigned int pSize, unsigned int pAddress);
error_t readBlockFromEEPROM(void *block, unsigned int pSize, unsigned int pAddress);

error_t loadAddressTableFromEEPROM(tempSensorTable_t *senorTable);
error_t writeAddressTableToEEPROM(tempSensorTable_t *sensorTable);

#endif /* EEPROMSAVER_H_ */