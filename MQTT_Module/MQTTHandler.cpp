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
void removeSpacesFromString(char *pString, uint16_t pStringLength);

//globals
WiFiClient wifiClient ;
PubSubClient mqttClient;
byte server[] = { 192, 168, 75, 50 }; //default Ip

extern tempSensorTable_t tempSensorTable;
extern config_t globalConfig;

error_t mqttInit(void){
	return mqttInit(server,MQTT_DEFAULT_PORT);
}

void mqttStop(void){
	mqttClient.disconnect();
	wifiClient.stop();
}

error_t mqttInit(uint8_t pServer[4], uint16_t pPort){
	
	mqttClient=PubSubClient(pServer, pPort, callback, wifiClient);
	error_t error=connectAndSubscribe();
	if (error){
		Serial.println(F("MQTT initial connection Failed!"));
		return error;
	}
	
	//maybe find a better location for initializing the output pin
	pinMode(2,OUTPUT);
	digitalWrite(2,LOW);
	return ERR_NO_ERR;
}

error_t checkConnectionAndReconnectMQTT(){

	if(!mqttClient.connected()) {
		Serial.println(F("No connection. Reconnect..."));
		return connectAndSubscribe();	
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
	
	snprintf(topic,sizeof(topic),"/asn/%i/switch/",globalConfig.id);
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
			removeSpacesFromString(sensorValue_s,sizeof(sensorValue_s));
			mqttClient.publish(topic_s,sensorValue_s);
						
		}
	}
}


void removeSpacesFromString(char *pString, uint16_t pStringLength){
	char *readPointer=pString;
	char *writePointer=pString;
	uint16_t i=0;
	char state=0;
		
	for(i=0; i<pStringLength;i++){ //walk thought the string
		
		switch (state){
			//find first empty space
			case 0:	if(*readPointer== ' '){
						writePointer=readPointer;
						state=1;
					}
					break;
			//find next character after space
			case 1: if(*readPointer != ' '){
						*writePointer=*readPointer;
						writePointer++;
						state=1;
					}
					break;
		
		}
		if(*readPointer == '\0'){ //reached the end of the string
			break;
		}
		
		readPointer++;
		
	}
	
	
}

//The callback function is call, if we receive any message
void callback(char* pTopic, byte* payload, unsigned int length) {

	char topic_s[MQTT_MAX_TOPIC_LENGTH];
		
	snprintf(topic_s,sizeof(topic_s),"/asn/%i/switch",globalConfig.id);
		
	if(!strcmp(pTopic,topic_s)){
		
		int state=atoi((char *)payload);
		digitalWrite(2,state);
	}
	else{
		snprintf(topic_s,sizeof(topic_s),"/asn/config/%i",globalConfig.id);	
		if(!strcmp(pTopic,topic_s)){
			Serial.println(F("config"));
		}
		else{
			Serial.println(F("Received unknown but registered topic:"));
			Serial.println(pTopic);
		}
	}
	Serial.println(pTopic);
	Serial.write(payload,length);
	Serial.println("");
}