/*
 * WifiCommunicator.h
 *
 * Created: 13.08.2014 12:19:48
 *  Author: gsteindl
 */ 


#ifndef WIFICOMMUNICATOR_H_
#define WIFICOMMUNICATOR_H_




#include "ErrorCodes.h"
#include "Config.h"


//Prototypes
error_t wifiComInit(char * pSsid, char* pPass);

#endif /* WIFICOMMUNICATOR_H_ */