/*
 * EEPROMSaver.h
 *
 * Created: 12.08.2014 16:19:50
 *  Author: gsteindl
 */ 


#ifndef EEPROMSAVER_H_
#define EEPROMSAVER_H_


#include "ErrorCodes.h"
#include "Config.h"
#include "TempSensor.h"
#include "ADConverter.h"



#define TEMP_SENSOR_TABLE_MEMORY_ADDRESS 0x01
#define ADC_TABLE_MEMORY_ADDRESS (TEMP_SENSOR_TABLE_MEMORY_ADDRESS + sizeof(tempSensorTable_t) + 1)
#define CONFIG_MEMORY_ADDRESS (ADC_TABLE_MEMORY_ADDRESS + MAX_ADC_VALUES + 1)

#define FLAG_REGION 0x00

//Flags
#define TEMP_SENS_TABLE_VALID_FLAG 0x01
#define CONFIG_VALID_FLAG 0x02
#define ADC_VALUE_TABLE_VALID_FLAG 0x04


error_t loadAddressTableFromEEPROM(tempSensorTable_t *pSensorTable);
error_t writeAddressTableToEEPROM(tempSensorTable_t *pSensorTable);

error_t loadADCTableFromEEPROM(uint8_t *pAdcValues);
error_t writeADCTableToEEPROM(uint8_t *pAdcValues);

error_t loadConfigFromEEPROM(config_t *pConfig);
error_t writeConfigToEEPROM(config_t *pConfig);
error_t deleteGlobalConfig(void);

#endif /* EEPROMSAVER_H_ */