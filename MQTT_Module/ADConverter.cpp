/*
 * ADConverter.cpp
 *
 * Created: 16.09.2014 10:17:19
 *  Author: gsteindl
 */ 

#include "ADConverter.h"

adcValue_t adcValues;


void adcInit(){
	//set pins
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);

	Serial.print(F("Read ADC table from EEPROM..."));
	if(loadADCTableFromEEPROM(adcValues.enabled)){
		//table not found in EEPROM
		turnAllAdcOff();
	}
	Serial.println(F("done."));
}

void adcRead(){
	
	for(uint8_t i=0; i< MAX_ADC_VALUES; i++){
		if(adcValues.enabled[i] == 1){
			adcValues.voltage[i]=analogRead(i)*4,88; // (5/1024)*1000 [mV] 
			delay(10);
		}
	}
}

/**
*	\brief Sets the state of every temperature sensor to OFF
*
*/
error_t turnAllAdcOff(){
	 
	for(int i=0; i < MAX_ADC_VALUES; i++){
		adcValues.enabled[i]=0;
	}
	writeADCTableToEEPROM(adcValues.enabled);
	return ERR_NO_ERR;
}

/**
*	\brief Sets the state of every temperature sensor to ON
*
*/
error_t turnAllAdcOn(){
	
	for(int i=0; i < MAX_ADC_VALUES; i++){
		adcValues.enabled[i]=1;
	}
	writeADCTableToEEPROM(adcValues.enabled);
	return ERR_NO_ERR;
}
error_t turnAdcOn(uint8_t pNm){
	
	if(pNm >= 0 && pNm <MAX_ADC_VALUES){
		adcValues.enabled[pNm]=1;

		writeADCTableToEEPROM(adcValues.enabled);
		return ERR_NO_ERR;
	}
	
	return ERR_ADC_PIN_OUT_OF_BOUND;
}

error_t turnAdcOff(uint8_t pNm){
	
	if(pNm >= 0 && pNm <MAX_ADC_VALUES){
		adcValues.enabled[pNm]=0;

		writeADCTableToEEPROM(adcValues.enabled);
		return ERR_NO_ERR;
	}
	
	return ERR_ADC_PIN_OUT_OF_BOUND;
}

void adcPrintTable(){
	
	for(uint8_t i=0; i< MAX_ADC_VALUES; i++){
		
		Serial.print(F("A")); Serial.print(i); Serial.print(F(" Enabled: "));
		if(adcValues.enabled[i] == 1)
			Serial.print(F("true"));
		else
			Serial.print(F("false"));
		
		Serial.print(F(" voltage [mV]: ")); Serial.println(adcValues.voltage[i]);
		
	}
	
}