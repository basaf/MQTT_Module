/**
 * Config.h
 *
 * \brief 
 * Created: 12.08.2014 12:43:52
 * Author: gsteindl
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


#define SIZE_ADDRESS_TABLE 5
#define PASS_SIZE_MAX 32			/*!< The maximum amount of bytes which are allowed for password*/
#define SSID_SIZE_MAX 32			/*!< The maximum amount of bytes which are allowed for the name of the SSID*/

struct tempSensor_s{
	uint8_t tempSensorID;				/*!< Every sensor needs an unique id.*/
	//uint32_t refreshPeriode;		/*!< Gives the time in ms in which the sensor is refreshed*/
	uint32_t sendPeriode;			/*!< Defines the time periode for the sending intervall*/
	uint32_t nextSendTime;
} typedef tempSensor_t;



struct config_s{					/*!<The struct holds the attributes, which can be configured.*/
	//version
	uint8_t version;				/*< Sets the version number which is required for the config file.*/
	//net
	uint8_t ssid[SSID_SIZE_MAX];			/*!< SSID of the WiFi-Network the sensor node has to connect. WPA2 and DHCP is used.*/
	uint8_t pass[PASS_SIZE_MAX];			/*!< Password for the WiFi-Network*/
	
	

} typedef config_t;



#endif /* CONFIG_H_ */