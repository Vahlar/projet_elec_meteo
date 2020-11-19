#ifndef gateway_functions_h
#define gateway_functions_h




#define LOCATION_MAC_ADDRESS           0

#define LOCATION_LORA_TXPOWER          10  //  int  
#define LOCATION_LORA_FREQUENCY        14  //  long 
#define LOCATION_LORA_SPREADINGFACTOR  18  //  int  
#define LOCATION_LORA_SIGNALBANDWIDTH  22  //  long 
#define LOCATION_LORA_CODINGRATE       26  //  int  
#define LOCATION_LORA_PREAMBLE         30  //  long 
#define LOCATION_LORA_SYNCWORD         34  //  int  
#define LOCATION_LORA_OCP              38  //  uint8_t 

#define LOCATION_WEB_SERVER           100  //  WEB_SERVER_MAX_LEN
#define LOCATION_WEB_LOGIN            200  // login SSID_MAX_LEN , passwd PASSWD_MAX_LEN

#define LOCATION_NETWORK_NB           299  //  uint8_t 
#define LOCATION_NETWORK              300  // ssid 32 , passwd 64

#define EEPROM_SIZE LOCATION_NETWORK+100*NETWORK_NB_MAX

#endif /* gateway_functions_h */




// tested but does not work
// https://github.com/ChuckBell/MySQL_Connector_Arduino/wiki.
