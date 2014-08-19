/*
 * MQTTHandler.cpp
 *
 * Created: 14.08.2014 09:15:16
 *  Author: gsteindl
 */ 

#include "MQTTHandler.h"
#include <stdio.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "TempSensor.h"
//#include "MemoryFree.h"
#include "Config.h"
#include <stdlib.h>

//Prototypes
void callback(char* topic, byte* payload, unsigned int length);
error_t connectAndSubscribe();

//globals
WiFiClient wifiClient ;
PubSubClient mqttClient;
byte server[] = { 192, 168, 75, 50 }; //default Ip

extern tempSensorTable_t tempSensorTable;
extern config_t globalConfig;

error_t mqttInit(void){
	return mqttInit(server,MQTT_DEFAULT_PORT);
}

error_t mqttInit(uint8_t pServer[4], uint16_t pPort){
	
	mqttClient=PubSubClient(pServer, pPort, callback, wifiClient);
	error_t error=connectAndSubscribe();
	if (error){
		Serial.println(F("MQTT initial connection Failed!"));
		return error;
	}
		
	return ERR_NO_ERR;
}

error_t checkConnectionAndReconnectMQTT(){

	if(!mqttClient.connected()) {
		Serial.println(F("Connection lost. Reconnect..."));
		connectAndSubscribe();	
	}
	
	return ERR_NO_ERR;
}

error_t connectAndSubscribe(void){
	
	char topic[MQTT_MAX_TOPIC_LENGTH];
	char clientName[10];
	
	snprintf(topic,sizeof(topic),"/asn/info/%i",globalConfig.id);
	snprintf(clientName,sizeof(clientName),"node_%i",globalConfig.id);
	
	if(!mqttClient.connect(clientName)){
		return ERR_MQTT_CONNECTION;
	}
	mqttClient.publish(topic,"Hello");
	
	//topic for subscribing
	snprintf(topic,sizeof(topic),"/asn/config/%i",globalConfig.id);
	mqttClient.subscribe(topic);
	
	return ERR_NO_ERR;
}

void mqttLoopFunction(){
	
		mqttClient.loop();

}

error_t mqttSendTemp(){

	char topic_s[MQTT_MAX_TOPIC_LENGTH];
	char sensorValue_s[MQTT_MAX_VALUE_STRING_LENGTH];
	
	for(int i=0; i<tempSensorTable.size;i++){
		if(tempSensorTable.tableEntry[i].state == 1){

			snprintf(topic_s,sizeof(topic_s),"/asn/%i/temp/%i/value",globalConfig.id,tempSensorTable.tableEntry[i].tempSensorID);
			
			dtostrf(tempSensorTable.tableEntry[i].sensorValue,sizeof(sensorValue_s)-3,2,sensorValue_s); //-3 -> for "-","."and "\0"
			mqttClient.publish(topic_s,sensorValue_s);
						
		}
	}
}


//The callback function is call, if we receive any message
void callback(char* topic, byte* payload, unsigned int length) {

	if(!strcmp(topic,"blackboard/1/out"))
	Serial.print("Unknown: ");
	
	Serial.write(payload,length);
	Serial.println("");
}