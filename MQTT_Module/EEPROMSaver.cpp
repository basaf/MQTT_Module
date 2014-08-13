/*
 * EEPROMSaver.cpp
 *
 * Created: 12.08.2014 16:18:24
 *  Author: gsteindl
 */ 


#include <EEPROM.h>
#include "EEPROMSaver.h"

//internal prototypes

error_t testEEPROMForValidTempSensorTable();
void setTempSensorTableFlagInEEPROM();
void clearTempSensorTableInEEPROM();

/************************************************************************/
/*! This function writes a whole block of data into the EEPROM at a
* specified address
*/
/************************************************************************/
error_t writeBlockToEEPROM(void *block, unsigned int pSize, unsigned int pAddress){
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
error_t readBlockFromEEPROM(void *block, unsigned int pSize, unsigned int pAddress){
	
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
error_t testEEPROMForValidTempSensorTable(){
	
	//TODO: implement version number
	uint8_t flags=EEPROM.read(FLAG_REGION);

	if( flags & TEMP_SENS_TABLE_VALID_FLAG)
		return ERR_NO_ERR;
	else
		return ERR_EEPROM_NO_VALID_TEMP_SENSOR_TABLE;
}

/************************************************************************/
/*! Clears a temperature sensor table in the EEPROM. Only clears the flag
* for a valid sensor table!
*/
/************************************************************************/
void clearTempSensorTableInEEPROM(){
	uint8_t flags=EEPROM.read(FLAG_REGION);
	flags=flags ^ TEMP_SENS_TABLE_VALID_FLAG;
	EEPROM.write(FLAG_REGION,flags);
}

/************************************************************************/
/*! Sets a temperature sensor flag in the EEPROM.
* 
*/
/************************************************************************/
void setTempSensorTableFlagInEEPROM(){
	uint8_t flags=EEPROM.read(FLAG_REGION);
	flags=flags | TEMP_SENS_TABLE_VALID_FLAG;
	EEPROM.write(FLAG_REGION,flags);
}

error_t loadAddressTableFromEEPROM(tempSensorTable_t *sensorTable){
	
	if(!testEEPROMForValidTempSensorTable()){
		readBlockFromEEPROM(sensorTable,sizeof(tempSensorTable_t),TEMP_SENSOR_TABLE_MEMORY_ADDRESS);
		return ERR_NO_ERR;
	}

	return ERR_EEPROM_NO_VALID_TEMP_SENSOR_TABLE;
}

error_t writeAddressTableToEEPROM(tempSensorTable_t *sensorTable){
		
	writeBlockToEEPROM(sensorTable,sizeof(tempSensorTable_t),TEMP_SENSOR_TABLE_MEMORY_ADDRESS);
	setTempSensorTableFlagInEEPROM();
	return ERR_NO_ERR;
	
}