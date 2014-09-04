/*
 * MQTTHandler.h
 *
 * Created: 14.08.2014 09:15:41
 *  Author: gsteindl
 */ 


#ifndef MQTTHANDLER_H_
#define MQTTHANDLER_H_

#include "ErrorCodes.h"
#include <inttypes.h>
#include <Arduino.h>

#define MQTT_DEFAULT_PORT 1883
#define MQTT_MAX_VALUE_STRING_LENGTH 15
#define MQTT_MAX_TOPIC_LENGTH 150
#define MQTT_TOPIC_ROOT "asn/" 

error_t mqttInit(void);
error_t mqttInit(uint8_t pServer[4], uint16_t pPort);
error_t checkConnectionAndReconnectMQTT();
error_t mqttSendTemp(void);
void mqttLoopFunction();
void mqttStop(void);


#endif /* MQTTHANDLER_H_ */