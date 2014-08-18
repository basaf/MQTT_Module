/*
 * CPPFile1.cpp
 *
 * Created: 12.08.2014 13:38:32
 *  Author: gsteindl
 */ 


#include "TempSensor.h"
#include "EEPROMSaver.h"
#include <OneWire.h>
#include <DallasTemperature.h>


//Global Variables
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
tempSensorTable_t tempSensorTable;

//internal Prototypes
error_t createAddressTable(void);
error_t mergeTables(tempSensorTable_t *eepromTable, tempSensorTable_t *currentTable);
tableEntry_t* serachAddressInTable(const uint8_t address[8], tempSensorTable_t *table);
tableEntry_t* serachIdInSensorTable(const uint8_t id, tempSensorTable_t *table);


/**
*	\brief Initializes the temperature sensors with the standard
*		resolution.
*/
void tempSenosrsInit(void){	
	tempSenosrsInit(DEFAULT_RESOLUTION);
	
}
/**
*	\brief Initializes the temperature sensors with a given resolution
*/
void tempSenosrsInit(const uint8_t resolution){
	sensors.begin();
	if(resolution >= 9 && resolution <=12){
		sensors.setResolution(resolution);
		Serial.println(F("Setting resolution to ")); Serial.print(resolution); Serial.println(F(" bit"));
	}
	else{
		//default is 10-bit
		Serial.println(F("Setting default resolution of temperature sensors"));
		sensors.setResolution(DEFAULT_RESOLUTION);
	}
	
	Serial.println(F("Create address table for OneWire sensors..."));
	createAddressTable();
	
	Serial.print(F("Temperature Sensors initialized @ pin ")); Serial.println(ONE_WIRE_BUS_PIN); 
}
/**
*	\brief Reads all connected temperature sensors.
*		
*		Table: DS18B20 Conversion Times and Resolution Settings
*		Resolution 				9 bit 	10 bit 	11 bit 	12 bit
*		Conversion Time (ms) 	93.75 	187.5 	375 	750
*		LSB (°C) 				0.5 	0.25 	0.125 	0.0625
*		See also: http://www.maximintegrated.com/en/app-notes/index.mvp/id/4377
*/
error_t tempSensorRead(void){
	
	sensors.requestTemperatures();
	for(int i=0; i<tempSensorTable.size; i++){
		if(sensors.isConnected(tempSensorTable.tableEntry[i].address)){
			tempSensorTable.tableEntry[i].sensorValue=sensors.getTempC(tempSensorTable.tableEntry[i].address);
		}
		else{
			tempSensorTable.tableEntry[i].sensorValue=999;
		}
	}
	return ERR_NO_ERR;
}

/**
*	\brief Prints all connected OneWire sensors with there index
		and current temperature value. 
*/
void tempSensorsPrintInfo(void){
		uint8_t address[8];
		float temp;
		
		uint8_t deviceCount=sensors.getDeviceCount();
		Serial.print("Found devices: ");
		Serial.println(deviceCount);
		Serial.println(" ");
		
		sensors.requestTemperatures();
			
		for(int i=0; i<deviceCount; i++){
			Serial.print("Device: ");
			Serial.println(i);
			if(sensors.getAddress(address, i)){

				temp=sensors.getTempC(address);
				
				Serial.print("Temp: ");
				Serial.print(temp);
				Serial.println(" ");
				//delay(100);
				
				Serial.print("Resolution: ");
				Serial.println(sensors.getResolution(address));
				Serial.println("");
				//delay(100);
			}
		
		}
	
}
/**
*	\brief Prints the current values in the temperature sensor table
*/
void tempSensorPrintTable(){
	
	for(int i=0; i< tempSensorTable.size; i++){
		for(int j=0; j<8;j++){
			Serial.print(tempSensorTable.tableEntry[i].address[j],HEX);
			Serial.print(F(":"));
		}
		
		Serial.print(" - "); Serial.print(tempSensorTable.tableEntry[i].tempSensorID);
		Serial.print(" - "); Serial.print(tempSensorTable.tableEntry[i].sensorValue);
		Serial.print(" - "); Serial.println(tempSensorTable.tableEntry[i].state);
	}
}


/**
*	\brief Creates a table with all sensors which are connected via OneWire.
*		
*		The table is created on the basis of a previous table which is stored
*		in the EEPROM. This helps to have a consistent assignment of the sensor ID
*		to the connected sensor. Otherwise this can change after a reset or if a 
*		sensor is removed.	
*/
error_t createAddressTable(void){
	uint8_t address[8];
	tempSensorTable.size=0;
	uint8_t index=0;
	tempSensorTable_t eepromTable;
	
	memset(&tempSensorTable,0,sizeof(tempSensorTable_t));
	
	int8_t deviceCount=sensors.getDeviceCount();
	for(int i=0; i<deviceCount; i++){
		if(sensors.getAddress(address, i)){
			memcpy(tempSensorTable.tableEntry[index].address,address,sizeof(address));
			tempSensorTable.tableEntry[index].tempSensorID=i;
			tempSensorTable.tableEntry[index].state=1;		//default: send temp sensor value
			++index;	
		}
	}
	tempSensorTable.size=index;
	
	if(!loadAddressTableFromEEPROM(&eepromTable)){
		//table found in EEPROM
		mergeTables(&eepromTable, &tempSensorTable);
	}
	
	writeAddressTableToEEPROM(&tempSensorTable);
	
	return ERR_NO_ERR;
}

/**
*	\brief Merge to tables together.
		An sensor address in the current table is searched in the table which is
		stored in the EEPROM. If the address is found there, the ID from the EEProm is 
		assigned. if this already exists in the new table, we have to swap the IDs.  
*/
error_t mergeTables(tempSensorTable_t *eepromTable, tempSensorTable_t *currentTable){
	tableEntry_t *tmpEntry_p;
	tableEntry_t *tmpEntry_p2;
	
	if(currentTable->size != 0){	//table exists already
		
		for(int i=0; i< currentTable->size;i++){
			uint8_t address[8];
			memcpy(address,currentTable->tableEntry[i].address,sizeof(address));
			
			tmpEntry_p=serachAddressInTable(address, eepromTable);
			if(tmpEntry_p != NULL){
				//so we found it
				uint8_t currentId=currentTable->tableEntry[i].tempSensorID;
				uint8_t oldId=tmpEntry_p->tempSensorID;
				
				//Find element with that id and replace its ID
				tmpEntry_p2=serachIdInSensorTable(oldId, currentTable);
				if(tmpEntry_p2 != NULL){
					tmpEntry_p2->tempSensorID=currentId;
				}
				
				//now we can overwrite the id
				currentTable->tableEntry[i].tempSensorID=oldId;
				
			}	
		}
	}
	return ERR_NO_ERR;
}

/**
*	\brief Searches an address in a sensor table
*/
tableEntry_t* serachAddressInTable(const uint8_t address[8], tempSensorTable_t *table){
	
	bool serachStatus;
	tableEntry_t *currentEntry;
	
	for(int i=0; i < table->size && serachStatus==false; i++){
		
		currentEntry= &(table->tableEntry[i]);
		serachStatus=true;
		
		//check all 8 Bytes
		for(int j=0; j<8 ;j++){
			if(address[j]!= currentEntry->address[j]){
				serachStatus=false;
				break;
			}		
		}
		
		if(serachStatus == true){
			return currentEntry;		
		}
	}
	return NULL;
}

/**
*	\brief Searches for an ID in a sensor table. 
*		A pointer to the found entry is returned.
*/
tableEntry_t * serachIdInSensorTable(const uint8_t id, tempSensorTable_t* table){

	for(int i=0; i< table->size; i++){
		if(table->tableEntry[i].tempSensorID==id){
			return &(table->tableEntry[i]);
		}
	}
	return NULL;
}
/**
*	\brief Sets the state of every temperature sensor to OFF
*		
*/
error_t turnAllTempSensorsOff(){
	
	for(int i=0; i < tempSensorTable.size;i++){
		tempSensorTable.tableEntry[i].state=0;
	}
	return ERR_NO_ERR;
}

/**
*	\brief Sets the state of every temperature sensor to ON
*
*/
error_t turnAllTempSensorsOn(){
	for(int i=0; i < tempSensorTable.size;i++){
		tempSensorTable.tableEntry[i].state=1;
	}
	return ERR_NO_ERR;
}


void testMergeTable(){
	
	tempSensorPrintTable();
	//tableEntry_t entryA1={{'A','\0',' ',' ',' ',' ',' ',' '},1,0};
	//tableEntry_t entryA2={{'B','\0',' ',' ',' ',' ',' ',' '},2,0};
	//tableEntry_t entryA3={{'C','\0',' ',' ',' ',' ',' ',' '},3,13};
	//tableEntry_t entryA4={{'D','\0',' ',' ',' ',' ',' ',' '},4,14};
		//
	//tableEntry_t entryB1={{'F','\0',' ',' ',' ',' ',' ',' '},1,0};
	//tableEntry_t entryB2={{'B','\0',' ',' ',' ',' ',' ',' '},2,0};
	//tableEntry_t entryB3={{'G','\0',' ',' ',' ',' ',' ',' '},3,0};
	//tableEntry_t entryB4={{'A','\0',' ',' ',' ',' ',' ',' '},4,0};		
	//
	//tempSensorTable_t tableA={ 4, 
			//entryA1, entryA2, entryA3, entryA4
	//};
	//tempSensorTable_t tableB={ 3,
		//entryB1, entryB2, entryB3, entryB4
	//};
	//Serial.println(F("Table A:"));
	//printTable(tableA);
	//Serial.println(" ");
	//Serial.println(F("Table B:"));
	//printTable(tableB);
	//
	//mergeTables(&tableA,&tableB);
	//
	//Serial.println(F("Table B new:"));
	//printTable(tableB);
}