/*
 * EEPROMSaver.cpp
 *
 * Created: 12.08.2014 16:18:24
 *  Author: gsteindl
 */ 



#include "EEPROMSaver.h"
#include <EEPROM.h>

//internal prototypes
error_t testEEPROMForValidFlag(uint8_t pFlag);
void setFlagInEEPROM(uint8_t pFlag);
void clearFlagInEEPROM(uint8_t pFlag);


error_t writeBlockToMyEEPROM(void *block, unsigned int pSize, unsigned int pAddress);
error_t readBlockFromMyEEPROM(void *block, unsigned int pSize, unsigned int pAddress);

/************************************************************************/
/*! This function writes a whole block of data into the EEPROM at a
* specified address
*/
/************************************************************************/
error_t writeBlockToMyEEPROM(void *block, unsigned int pSize, unsigned int pAddress){
	void *ptr;
	
	ptr=block;
	for(unsigned int i=0; i < pSize; i++){
		
		ptr=block+i;
		EEPROM.write(pAddress+i,*(char*)ptr);
	}
	return ERR_NO_ERR;
}
/************************************************************************/
/*!This function retrieves a block of data from the EEPROM at a 
* specified address.
*/
/************************************************************************/
error_t readBlockFromMyEEPROM(void *block, unsigned int pSize, unsigned int pAddress){
	
	void *ptr=block;
	
	for(unsigned int i=0; i < pSize; i++){
		
		ptr=block+i;
		*(char*)ptr=EEPROM.read(pAddress+i);
	}
	
	
	return ERR_NO_ERR;
}


/************************************************************************/
/*! Test if a temperature sensor table is present in the EEPROM
*  
*/
/************************************************************************/
error_t testEEPROMForValidFlag( uint8_t pFlag )
{
	
	//TODO: implement version number
	uint8_t flags=EEPROM.read(FLAG_REGION);

	if( flags & pFlag)
		return ERR_NO_ERR;
	else
		return ERR_EEPROM_NO_VALID_FLAG;
}

/************************************************************************/
/*! Clears a flag in the EEPROM.
*/
/************************************************************************/
void clearFlagInEEPROM(uint8_t pFlag)
{
	uint8_t flags=EEPROM.read(FLAG_REGION);
	flags=flags ^ pFlag;
	EEPROM.write(FLAG_REGION,flags);
}

/************************************************************************/
/*! Sets a temperature sensor flag in the EEPROM.
* 
*/
/************************************************************************/
void setFlagInEEPROM(uint8_t pFlag)
{
	uint8_t flags=EEPROM.read(FLAG_REGION);
	flags=flags | pFlag;
	EEPROM.write(FLAG_REGION,flags);
}

error_t loadADCTableFromEEPROM(uint8_t *pAdcValues){
	
	if(!testEEPROMForValidFlag(ADC_VALUE_TABLE_VALID_FLAG)){
		readBlockFromMyEEPROM(pAdcValues,MAX_ADC_VALUES,ADC_TABLE_MEMORY_ADDRESS);
		return ERR_NO_ERR;
	}

	return ERR_EEPROM_NO_VALID_ADC_TABLE;
}
error_t writeADCTableToEEPROM(uint8_t *pAdcValues){
	writeBlockToMyEEPROM(pAdcValues,MAX_ADC_VALUES, ADC_TABLE_MEMORY_ADDRESS);
	setFlagInEEPROM(ADC_VALUE_TABLE_VALID_FLAG);
	return ERR_NO_ERR;
}

error_t loadAddressTableFromEEPROM(tempSensorTable_t *pSensorTable){
	
	if(!testEEPROMForValidFlag(TEMP_SENS_TABLE_VALID_FLAG)){
		readBlockFromMyEEPROM(pSensorTable,sizeof(tempSensorTable_t),TEMP_SENSOR_TABLE_MEMORY_ADDRESS);
		return ERR_NO_ERR;
	}

	return ERR_EEPROM_NO_VALID_TEMP_SENSOR_TABLE;
}

error_t writeAddressTableToEEPROM(tempSensorTable_t *pSensorTable){
		
	writeBlockToMyEEPROM(pSensorTable,sizeof(tempSensorTable_t),TEMP_SENSOR_TABLE_MEMORY_ADDRESS);
	setFlagInEEPROM(TEMP_SENS_TABLE_VALID_FLAG);
	return ERR_NO_ERR;
	
}

error_t writeConfigToEEPROM(config_t *pConfig){
		
	writeBlockToMyEEPROM(pConfig,sizeof(config_t),CONFIG_MEMORY_ADDRESS);
	setFlagInEEPROM(CONFIG_VALID_FLAG);
	return ERR_NO_ERR;
	
}
error_t loadConfigFromEEPROM(config_t *pConfig){
	
	if(!testEEPROMForValidFlag(CONFIG_VALID_FLAG)){
		readBlockFromMyEEPROM(pConfig,sizeof(config_t),CONFIG_MEMORY_ADDRESS);		
		return ERR_NO_ERR;
	}

	return ERR_EEPROM_NO_VALID_CONFIG;
	
}

error_t deleteGlobalConfig(){
	
	clearFlagInEEPROM(CONFIG_VALID_FLAG);
}