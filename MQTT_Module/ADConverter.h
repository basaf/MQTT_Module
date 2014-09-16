/*
 * ADConverter.h
 *
 * Created: 16.09.2014 10:17:45
 *  Author: gsteindl
 */ 


#ifndef ADCONVERTER_H_
#define ADCONVERTER_H_

#include <Arduino.h>
#include "ErrorCodes.h"
#include "EEPROMSaver.h"

#define MAX_ADC_VALUES 6

struct adcValue_s{
	float voltage[MAX_ADC_VALUES];
	uint8_t enabled[MAX_ADC_VALUES]; 
} typedef adcValue_t;

void adcInit(void);
void adcRead(void);
error_t turnAllAdcOn();
error_t turnAllAdcOff();
void adcPrintTable();
error_t turnAdcOn(uint8_t pNm);
error_t turnAdcOff(uint8_t pNm);


#endif /* ADCONVERTER_H_ */