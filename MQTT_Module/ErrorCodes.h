/*
 * ErrorCodes.h
 *
 * Created: 12.08.2014 16:25:50
 *  Author: gsteindl
 */ 


#ifndef ERRORCODES_H_
#define ERRORCODES_H_


//DEBUG Macro
#ifdef DEBUG
#define _DEBUG(s) Serial.println(s);
#else
#define _DEBUG(s)
#endif

typedef unsigned int error_t;

//Codes
#define ERR_NO_ERR			0x0

//UDPCom
#define ERR_UDP_SSID_LENGTH 0x1001
#define ERR_UDP_PASS_LENGTH 0x1002
#define ERR_UDP_CONNECTION  0x1003

//EEPROM
#define ERR_EEPROM_NO_VALID_TEMP_SENSOR_TABLE 0x2001
#define ERR_EEPROM_LOAD 0x2002




#endif /* ERRORCODES_H_ */