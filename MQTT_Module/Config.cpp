/*
 * Config.cpp
 *
 * Created: 14.08.2014 11:07:09
 *  Author: gsteindl
 */ 
#include <Arduino.h>
#include "Config.h"
#include "TempSensor.h"
#include "EEPROMSaver.h"

//Prototyps
void resetConfig(config_t *pConfig);
void sendConfig(config_t *pConfig);
error_t checkForSerialControlString();
error_t configMode(void);
error_t checkGlobalConfig(void);

//Globals
config_t globalConfig;


void configInit(){
	
	//set config Pin as input and use internal pull-up resistor
	pinMode(CONFIG_PIN,INPUT);
	digitalWrite(CONFIG_PIN,HIGH);
	delay(100);
	
	uint8_t configPinState=digitalRead(CONFIG_PIN);

	if(loadConfigFromEEPROM(&globalConfig)){
		//Fehler beim laden der Config.
		Serial.println(F("Couldn't load config from EEPROM!"));
		//TODO: Automatisch die config reseten!!!
		configMode();
	}
	
	if(!configPinState){
		//It is pulled to ground so we switch to config mode
		Serial.println(F("Forced configuration mode"));
		configMode();
	}
	
	
}

/*!
*	\brief This Mode interacts with the user to set the config parameters
*/
error_t configMode(){
	error_t state;
	uint8_t configChecked=0;
	
	Serial.println(F("Entered configuration mode!"));
	Serial.println(F("Type 'exit' to leave this mode."));
	Serial.println(F("Type 'help' for more information..."));
	while(state != EXIT){
		state=checkForSerialControlString();
		delay(300);
		//if we have to proceed we have to check if the config struct in the EEPROM is valid
		if(state == EXIT){
			if(checkGlobalConfig()){
				Serial.println(F("The config struct in the EEPROM is not valid!"));
				Serial.println(F("Please check the config..."));
				//Reset the state from EXIT, so we keep in the config mode
				state=0;
			}
		}
	}
	return ERR_NO_ERR;	
}

/************************************************************************/
/*! Wait for a control string which is sent over the serial port.
*/
/************************************************************************/
error_t checkForSerialControlString(){
	char buffer[MAX_SERIAL_BUFFER_SIZE];
	unsigned int i=0;
	char c;
	
	if(Serial.available()){
		Serial.print(F("Command: "));
		while(Serial.available() && i < (MAX_SERIAL_BUFFER_SIZE-1)){
			c=Serial.read();
			if(c != '\t' && c != '\r' && c!= '\n'){
				buffer[i]=c;
				i++;
			}
		}
		buffer[i]=0;
		
		Serial.println(buffer);
		
		if(!strcmp(buffer, "reset config")){	
			resetConfig(&globalConfig);
		}
		else if(!strcmp(buffer, "delete config")){
			deleteGlobalConfig();
		}
		else if(!strcmp(buffer, "show config")){
			sendConfig(&globalConfig);
		}
		else if(!strcmp(buffer, "set sensors states")){
			turnAllTempSensorsOn();
		}
		else if(!strcmp(buffer, "rm sensors states")){
			turnAllTempSensorsOff();
		}
		else if(!strcmp(buffer, "set id")){
			
		}
//
		//else if(!strcmp(buffer, "set debug")){
			//debug=true;
		//}
		else if(!strcmp(buffer, "exit")){
			return EXIT;
		}
		else if(!strcmp(buffer, "help")){
			Serial.println(F("Commands:"));
			Serial.println(F("help - shows this list of commands"));
			Serial.println(F("show config - prints the current configuration"));
			Serial.println(F("reset config - resets the config to the default values"));
			Serial.println(F("delete config - deletes the whole config from the EEPROM"));
			Serial.println(F("set sensors states - every sensor value will be transmitted"));
			Serial.println(F("rm sensors states - no sensor value will be transmitted"));
			Serial.println(F("set id - sets the global ID of the node"));
			//Serial.println(F("set debug - prints the current sensor values when the are sent"));
			//Serial.println(F("rm debug - disable the printing of they sensor values"));
			Serial.println(F("exit - leaves the config mode"));
		}
		else{
			Serial.println(F("Command unknown!"));
			Serial.println(F("Type 'help' for available commands!"));
		}
	
	}
	return ERR_NO_ERR;
}



/*!
*	\brief sends the stored config over the serial interface
*
*/
void sendConfig(config_t *pConfig){
	Serial.print(F("SSID: "));
	Serial.println(pConfig->ssid);
	Serial.print(F("Pass: "));
	Serial.println(pConfig->pass);
	Serial.print(F("Send interval: "));
	Serial.println(pConfig->sendInterval);
	Serial.print(F("Resolution: "));
	Serial.println(pConfig->resolution);
	
}

/*!
*	\brief Sets the config to its default values and stores it in the EEPROM
*
*	Default Values:	SSID="ArduinoNet"
*					Password="ArduinoNet"
*					Send interval = 60s
*					All Sensors turned off
*					Resolution=10
*					Version=1
*					id=99
*/
void resetConfig(config_t *pConfig){
	
	snprintf(pConfig->pass,PASS_LENGTH_MAX,"ArduinoNet");
	snprintf(pConfig->ssid,SSID_LENGTH_MAX,"ArduinoNet");
	pConfig->resolution=10;
	pConfig->sendInterval=60;
	pConfig->version=1;
	pConfig->id=99;
	//turn all sensors Off
	turnAllTempSensorsOn();
	writeConfigToEEPROM(pConfig);
	Serial.println(F("Config is reseted to its default values"));
}

/*!
*
*/
error_t setGlobalId(uint8_t id){
	
	globalConfig.id=id;
	
	return ERR_NO_ERR;
}

/*!
* \brief This function checks the config in the EEPROM if every parameter has a
*		valid value
*/
error_t checkGlobalConfig(){
	//TODO
	return ERR_NO_ERR;
}