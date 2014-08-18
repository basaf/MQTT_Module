/*
 * MQTT_Module.cpp
 *
 * Created: 11.08.2014 15:25:45
 *  Author: gsteindl
 */ 

#define F_CPU 16000000

#include <Arduino.h>
#include <stdlib.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "TempSensor.h"
#include "Config.h"
#include "WifiCommunicator.h"
#include "MQTTHandler.h"
#include "MemoryFree.h"

//prototypes
void setup();
void loop();

//Globals
extern config_t globalConfig;

char ssid[] = "ArduinoNet";     //  your network SSID (name)
char pass[] = "ArduinoNet";  // your network password


void setup() {

	Serial.begin(9600);
	
	
	Serial.print(F("Free memory1: "));
	Serial.println(freeMemory());
	delay(200);
	
	configInit();
	tempSenosrsInit(globalConfig.resolution);
	wifiComInit(globalConfig.ssid, globalConfig.pass);
	//wifiComInit("ArduinoNet","ArduinoNet");
	mqttInit();

	Serial.println(F("Initialization done"));
}
void loop() {

	checkConnectionAndReconnectMQTT();
		
	tempSensorRead();
	tempSensorPrintTable();
 	
	mqttSendTemp();
	
	Serial.println("------------");
	delay(7000);
	mqttLoopFunction();	//MQTT client loop processing for keep alive messages
}
