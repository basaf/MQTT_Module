/*
 * MQTT_Module.cpp
 *
 * Created: 11.08.2014 15:25:45
 *  Author: gsteindl
 */ 

#define F_CPU 16000000

#include <Arduino.h>
#include <stdlib.h>

#include "TempSensor.h"


//prototypes
void setup();
void loop();

void setup() {

	Serial.begin(9600);
	tempSenosrsInit();
	Serial.println("Start");
}
void loop() {

	testMergeTable();
	tempSensorsPrintInfo();
	
	Serial.println("------------");
	delay(7000);
}