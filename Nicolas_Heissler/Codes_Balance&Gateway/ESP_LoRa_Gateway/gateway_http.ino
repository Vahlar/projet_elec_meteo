#include "gateway_http.h"

// ==============================
int8_t write_data_to_DB(String httpRequestData) {
  // httpRequestData : Data to send with HTTP POST
  HTTPClient  http;
  int         httpResponseCode;
  String      httpResponse;
  int         str_begin, str_end;
  int8_t      idx = 0 ;
  char        buf[WEB_SERVER_MAX_LEN+WEB_URL_MAX_LEN];

  // Your Domain name with URL path or IP address with path
  //http.begin("http://192.168.0.102:80/xxxx/bees/http_post.php");
  //http.begin("192.168.0.102", 80, "/xxxx/bees/http_post.php");
  //http.begin(web_server, 80, web_url);
  sprintf(buf,"%s%s",web_server,add_datas_url);
  http.begin(buf);
  // if "web_url" is protected by htaccess
  if ( web_login != "" )
    http.setAuthorization(web_login, web_passwd);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");         
  // Send HTTP POST request
  httpResponseCode = http.POST(httpRequestData); 
  //DEBUGPRINT3("HTTP Response code: "); DEBUGPRINTLN3(httpResponseCode);

  switch ( httpResponseCode ) {
    case HTTP_CODE_OK :
      httpResponse = http.getString();
      DEBUGPRINTLN3(" ====== httpResponse ==========");
      DEBUGPRINTLN3(httpResponse);
      DEBUGPRINTLN3(" ====== httpResponse ==========");
      
      for ( idx = ENR_NB-1 ; idx > 0 ; idx -- ) {
        memcpy( &enr[idx].mess[0], &enr[idx-1].mess[0], LENT);
      }

      if ( httpResponse.indexOf("Pas de capteur") >= 0 ) { // "Pas de capteur "
        sprintf(enr[0].mess,"%s",NO_SENSOR);
        sprintf(enr[0].object,"%c %02X:%02X:%02X:%02X:%02X:%02X",payload[0], payload[1], payload[2], payload[3], payload[4], payload[5], payload[6]);
      } else if ( httpResponse.indexOf("INSERT_DONE") >= 0 ) { //"INSERT_DONE $num_ruche \n";
        str_begin = httpResponse.indexOf("INSERT_DONE") + 11; 
        str_end  = httpResponse.indexOf("*--*");
        sprintf(enr[0].mess,"%s",DATA_OK);
        sprintf(enr[0].object,"%s %s",HIVE, httpResponse.substring(str_begin, str_end) );       
      } else if ( httpResponse.indexOf("NO INSERT") >= 0 ) { //"NO INSERT delta too small $num_ruche \n";
        str_begin = httpResponse.indexOf("NO INSERT") + 25;
        str_end  = httpResponse.indexOf("*--*");
        sprintf(enr[0].mess,"%s",DATA_KO);
        sprintf(enr[0].object,"%s %s",HIVE, httpResponse.substring(str_begin, str_end) );
      }
      sprintf(enr[0].rssi,"RSSI %d dBm",LoRa.packetRssi());
      sprintf(enr[0].snr,"SNR  %d dB",LoRa.packetSnr());
      oled_print(OC16B2,enr[0].mess, enr[0].object, enr[0].rssi, enr[0].snr);
      previousTime = millis();  

//      if ( httpResponse.indexOf("Pas de capteur") >= 0 ) { // "Pas de capteur "
//        sprintf(buf,"%c %02X:%02X:%02X:%02X:%02X:%02X",payload[0], payload[1], payload[2], payload[3], payload[4], payload[5], payload[6]);
//        oled_print(OC16, NO_SENSOR, buf , "RSSI " + (String)LoRa.packetRssi() + " dBm", "SNR " + (String)LoRa.packetSnr() + " dB");
//        previousTime = millis();
//      } else if ( httpResponse.indexOf("INSERT_DONE") >= 0 ) { //"INSERT_DONE $num_ruche \n";
//        str_begin = httpResponse.indexOf("INSERT_DONE") + 11; 
//        str_end  = httpResponse.indexOf("*--*");
//        sprintf(buf,"%s %s",HIVE, httpResponse.substring(str_begin, str_end) );
//        oled_print(OC16B2, DATA_OK, buf , "RSSI " + (String)LoRa.packetRssi() + " dBm", "SNR " + (String)LoRa.packetSnr() + " dB");
//        previousTime = millis();        
//      } else if ( httpResponse.indexOf("NO INSERT") >= 0 ) { //"NO INSERT delta too small $num_ruche \n";
//        str_begin = httpResponse.indexOf("NO INSERT") + 25;
//        str_end  = httpResponse.indexOf("*--*");
//        sprintf(buf,"%s %s",HIVE, httpResponse.substring(str_begin, str_end) );
//        oled_print(OC16B2, DATA_KO, buf , "RSSI " + (String)LoRa.packetRssi() + " dBm", "SNR " + (String)LoRa.packetSnr() + " dB");
//        previousTime = millis();       
//      }
      break;
    case HTTP_CODE_UNAUTHORIZED :
      DEBUGPRINTLN3("Authentication failed (HTTP Response code: 401)");
    case HTTP_CODE_NOT_FOUND :
      DEBUGPRINTLN3("Not found (HTTP Response code: 404)");
    default :
      Serial.printf("HTTP failed, error: %s (%d)\n", http.errorToString(httpResponseCode).c_str(),httpResponseCode);
      for ( idx = ENR_NB-1 ; idx > 0 ; idx -- ) {
        memcpy( &enr[idx].mess[0], &enr[idx-1].mess[0], LENT);
      }
      sprintf(enr[0].mess,"http:%d %s",httpResponseCode,http.errorToString(httpResponseCode).c_str());
      sprintf(enr[0].object,"%c %02X:%02X:%02X:%02X:%02X:%02X",payload[0], payload[1], payload[2], payload[3], payload[4], payload[5], payload[6]);
      sprintf(enr[0].rssi,"RSSI %d dBm",LoRa.packetRssi());
      sprintf(enr[0].snr,"SNR  %d dB",LoRa.packetSnr());
      oled_print(OC16,enr[0].mess, enr[0].object, enr[0].rssi, enr[0].snr);
      previousTime = millis(); 
      break;
  }

  // Free resources
  http.end();
}

// ==============================
String build_request() {
  String request = "data=";
  uint8_t idx = 0;

  for ( idx = 0 ; idx < payload_lenght ; idx++ ) {
    request += payload[idx];
    request += ".";
  }

  for ( idx = 0 ; idx < 5 ; idx++ ) {
    request += mac_gateway[idx];
    request += ".";
  }
  request += mac_gateway[idx];
  
  DEBUGPRINT3("build_request : "); DEBUGPRINTLN3(request);

  return request;
}

// ==============================
int8_t send_reboot_mess() {
  HTTPClient  http;
  int         httpResponseCode;
  String      httpResponse;
  char        buf[WEB_SERVER_MAX_LEN+WEB_URL_MAX_LEN];

  sprintf(buf,"%s%s",web_server,reboot_mess_url);
  http.begin(buf);
  // if "web_url" is protected by htaccess
  if ( web_login != "" )
    http.setAuthorization(web_login, web_passwd);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");         
  // Send HTTP POST request
  sprintf(buf,"mac=%02X%02X%02X%02X%02X%02X",
          mac_gateway[0], mac_gateway[1], mac_gateway[2], mac_gateway[3], mac_gateway[4], mac_gateway[5]);
  httpResponseCode = http.POST(buf); 
  //DEBUGPRINT3("HTTP Response code: "); DEBUGPRINTLN3(httpResponseCode);

  switch ( httpResponseCode ) {
    case HTTP_CODE_OK :
      httpResponse = http.getString();
      DEBUGPRINTLN3(" ====== httpResponse reboot_mess ==========");
      DEBUGPRINTLN3(httpResponse);
      DEBUGPRINTLN3(" ====== httpResponse reboot_mess ==========");
      break;
    case HTTP_CODE_UNAUTHORIZED :
      Serial.println("Authentication failed (HTTP Response code: 401)");
      oled_print(OC16,"HTTP", "Authentication failed", "Response code: 401", "htaccess ???");
      delay(3000); previousTime = millis(); 
      break;
    case HTTP_CODE_NOT_FOUND :
      Serial.println("Not found (HTTP Response code: 404)");
      oled_print(OC16,"HTTP", "Not found", "Response code: 404", "htaccess ???");
      delay(3000); previousTime = millis(); 
      break;
    default :
      Serial.printf("HTTP failed, error: %s (%d)\n", http.errorToString(httpResponseCode).c_str(),httpResponseCode);
      oled_print(OC16,"HTTP", "failed, error : ", String(http.errorToString(httpResponseCode).c_str()) ,String(httpResponseCode) );
      delay(3000); previousTime = millis(); 
      break;
  }

  // Free resources
  http.end();
}

// ==============================
int8_t send_wifi_connect_mess(char *network_ssid, enum wc wifi_connected) { //wc{WIFI_FAILED, WIFI_CONNECTED}
  HTTPClient  http;
  int         httpResponseCode;
  String      httpResponse;
  char        buf[WEB_SERVER_MAX_LEN+WEB_URL_MAX_LEN];

  sprintf(buf,"%s%s",web_server, wifi_mess_url);
  http.begin(buf);
  // if "web_url" is protected by htaccess
  if ( web_login != "" )
    http.setAuthorization(web_login, web_passwd);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");         
  // Send HTTP POST request
  sprintf(buf,"mac=%02X%02X%02X%02X%02X%02X&wifi=%s&connected=%d",
          mac_gateway[0], mac_gateway[1], mac_gateway[2], mac_gateway[3], mac_gateway[4], mac_gateway[5],
          network_ssid, wifi_connected);
  httpResponseCode = http.POST(buf); 
  //DEBUGPRINT3("HTTP Response code: "); DEBUGPRINTLN3(httpResponseCode);

  switch ( httpResponseCode ) {
    case HTTP_CODE_OK :
      httpResponse = http.getString();
      DEBUGPRINTLN3(" ====== httpResponse wifi_connect_mess ==========");
      DEBUGPRINTLN3(httpResponse);
      DEBUGPRINTLN3(" ====== httpResponse wifi_connect_mess ==========");
      break;
    case HTTP_CODE_UNAUTHORIZED :
      Serial.println("Authentication failed (HTTP Response code: 401)");
      oled_print(OC16,"HTTP", "Authentication failed", "Response code: 401", "htaccess ???");
      delay(3000); previousTime = millis(); 
      break;
    case HTTP_CODE_NOT_FOUND :
      Serial.println("Not found (HTTP Response code: 404)");
      oled_print(OC16,"HTTP", "Not found", "Response code: 404", "htaccess ???");
      delay(3000); previousTime = millis(); 
      break;
    default :
      Serial.printf("HTTP failed, error: %s (%d)\n", http.errorToString(httpResponseCode).c_str(),httpResponseCode);
      oled_print(OC16,"HTTP", "failed, error : ", String(http.errorToString(httpResponseCode).c_str()) ,String(httpResponseCode) );
      delay(3000);  previousTime = millis(); 
      break;
  }

  // Free resources
  http.end();
}
