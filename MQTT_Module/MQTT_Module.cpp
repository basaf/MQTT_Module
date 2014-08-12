/*
 * MQTT_Module.cpp
 *
 * Created: 11.08.2014 15:25:45
 *  Author: gsteindl
 */ 

#define F_CPU 16000000

#include <Arduino.h>
#include <stdlib.h>

void setup();
void loop();
void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(9600);
}
void loop() {
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);
	Serial.println("Hello World!");
}