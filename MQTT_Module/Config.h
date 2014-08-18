/**
 * Config.h
 *
 * \brief 
 * Created: 12.08.2014 12:43:52
 * Author: gsteindl
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <inttypes.h>
#include "ErrorCodes.h"

#define CONFIG_PIN 5
#define MAX_SERIAL_BUFFER_SIZE 64
//#define MAX_RECEIVE_BUFFER_SIZE 25
#define PASS_LENGTH_MAX 32			/*!< The maximum amount of bytes which are allowed for password*/
#define SSID_LENGTH_MAX 32			/*!< The maximum amount of bytes which are allowed for the name of the SSID*/

//struct tempSensor_s{
	//uint8_t tempSensorID;				/*!< Every sensor needs an unique id.*/
	////uint32_t refreshPeriode;		/*!< Gives the time in ms in which the sensor is refreshed*/
	//uint32_t sendPeriode;			/*!< Defines the time periode for the sending intervall*/
	//uint32_t nextSendTime;
//} typedef tempSensor_t;



struct config_s{					/*!<The struct holds the attributes, which can be configured.*/
	//version
	uint8_t version;				/*< Sets the version number which is required for the config file.*/
	//net
	char ssid[SSID_LENGTH_MAX];			/*!< SSID of the WiFi-Network the sensor node has to connect. WPA2 and DHCP is used.*/
	char pass[PASS_LENGTH_MAX];			/*!< Password for the WiFi-Network*/
	//
	uint8_t resolution;
	uint8_t sendInterval;
	uint8_t id;						/*!< the global id of the arduino modules*/
	

} typedef config_t;


//Prototype
void configInit(void);
error_t setGlobalId(uint8_t id);

#endif /* CONFIG_H_ */