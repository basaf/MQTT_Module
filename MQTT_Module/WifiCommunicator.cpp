/*
 * WifiCommunicator.cpp
 *
 * Wifi shield uses Pin 11,12,13 and 7
 * Created: 13.08.2014 12:19:19
 *  Author: gsteindl
 */ 


#include "WifiCommunicator.h"
#include <WiFi.h>

//internal Prototypes
void printTheWifiStatus();
	

/************************************************************************/
/*! Initialize the WiFi-Shield with ssid, password and local
 port. The ip is set by the DHCP server.
*/
/************************************************************************/	
error_t wifiComInit(char *pSsid, char *pPass){

	int status = WL_IDLE_STATUS;
	byte tries=0; //after some tries to connect we will abort
	
	if(strlen(pSsid) >= SSID_LENGTH_MAX) //strlen doesn't count the 0-termination
		return ERR_UDP_SSID_LENGTH;
		
	if(strlen(pPass) >= PASS_LENGTH_MAX)
		return ERR_UDP_PASS_LENGTH;


	// check for the presence of the shield:
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println(F("WiFi shield not present"));
		// don't continue:
		delay(1000);
		while(true);
	}


	// attempt to connect to Wifi network:
	while ( status != WL_CONNECTED) {
		if(tries >= 3){
			//error. we can't connect
			return ERR_UDP_CONNECTION;
		}
		
		Serial.print(F("Attempting to connect to SSID: "));
		Serial.println(pSsid);
		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		status = WiFi.begin(pSsid,pPass);
		// wait 10 seconds for connection:
		delay(8000);
		tries++;
	}
	
	//Show that everything is working 
	printTheWifiStatus();
	
	
	return ERR_NO_ERR;
	
}

void printTheWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print(F("SSID: "));
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress address = WiFi.localIP();
	Serial.print(F("IP: "));
	Serial.println(address);
	
	//print net mask
	address=WiFi.subnetMask();
	//Serial.print(F("net mask: "));
	//Serial.println(address);
	
	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print(F("RSSI:"));
	Serial.print(rssi);
	Serial.println(F(" dBm"));
}