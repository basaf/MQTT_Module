/*
 * MQTTHandler.cpp
 *
 * Created: 14.08.2014 09:15:16
 *  Author: gsteindl
 */ 

#include "MQTTHandler.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "TempSensor.h"
#include "MemoryFree.h"
#include "Config.h"
#include <string.h>

//Prototypes
char *ftoa(char *a, double f, int precision);
void callback(char* topic, byte* payload, unsigned int length);

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
	if (mqttClient.connect("arduinoClient")){	//TODO: CHANGE
		Serial.println(F("MQTT initialized"));
		mqttClient.publish("blackboard/arduino1/out","Hello I am Arduino 1");
	}
	else{
		Serial.println(F("MQTT initial connection Failed!"));
	}
	return ERR_NO_ERR;
}

error_t checkConnectionAndReconnectMQTT(){
	
	if(!mqttClient.connected()) {
		Serial.println(F("CONNECTION LOST"));
		mqttClient.connect("arduinoClient");	//TODO: CHANGE
		mqttClient.publish("blackboard/arduino1/out","Hello I am Arduino 1");
		//mqttClient.subscribe("blackboard/#");
		Serial.println(F("subscribed to blackboard/#"));
	}
	
	return ERR_NO_ERR;
}

void mqttLoopFunction(){
	
		mqttClient.loop();

}

error_t mqttSendTemp(){
	String valueString;
	String topicString;
	String globalIdString,sensorIdString;
	char *topic;
	char sensorValue_cA[10];
	
	globalIdString=String(globalConfig.id);
	
	Serial.print(F("Free memory topic1: "));
	Serial.println(freeMemory());	
	
	
	
	for(int i=0; i<tempSensorTable.size;i++){
		if(tempSensorTable.tableEntry[i].state == 1){
			topicString=String(MQTT_TOPIC_ROOT);
			topicString=topicString+globalIdString+"/temp/";
			sensorIdString=String(tempSensorTable.tableEntry[i].tempSensorID);
			topicString=topicString+sensorIdString+"/value";
			topic=(char*)topicString.c_str();
			Serial.println(topic);
			
			ftoa(sensorValue_cA, tempSensorTable.tableEntry[i].sensorValue, 2);
			//mqttClient.publish(topic,);
			mqttClient.publish(topic,sensorValue_cA);
			
			Serial.print(F("Free memory topic1: "));
			Serial.println(freeMemory());
			
		}
	}
}

/*!
*	\brief Converts a float into a string
*/
char *ftoa(char *a, double f, int precision){
	long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
	
	char *ret = a;
	long heiltal = (long)f;
	itoa(heiltal, a, 10);
	while (*a != '\0') a++;
	*a++ = '.';
	long desimal = abs((long)((f - heiltal) * p[precision]));
	itoa(desimal, a, 10);
	return ret;
}

//The callback function is call, if we receive any message
void callback(char* topic, byte* payload, unsigned int length) {

	if(!strcmp(topic,"blackboard/1/out"))
	Serial.print("Unknown: ");
	
	Serial.write(payload,length);
	Serial.println("");

	
}