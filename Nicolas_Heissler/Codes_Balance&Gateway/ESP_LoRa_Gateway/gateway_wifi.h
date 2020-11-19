#ifndef gateway_wifi_h
#define gateway_wifi_h

enum wc {WIFI_FAILED, WIFI_CONNECTED};

#define NETWORK_NB_MAX  5
#define SSID_MAX_LEN    32
#define PASSWD_MAX_LEN  64

#include <WiFi.h>
WiFiClient client;

int8_t network_nb; // = 5;
char ssids[NETWORK_NB_MAX][SSID_MAX_LEN]; 
char passwds[NETWORK_NB_MAX][PASSWD_MAX_LEN]; 
const unsigned long intervalWiFi = 60000 / FREQ_COEF; // 60000 = 1 minutes in milliseconds

char web_login[SSID_MAX_LEN]; 
char web_passwd[PASSWD_MAX_LEN];


#endif /* gateway_wifi_h */
