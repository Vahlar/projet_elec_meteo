#include "gateway_ble.h"
#include "gateway_http.h"

// ==============================
void create_ble_uart (uint8_t * macSTA, String info) {
  // Create the BLE Device
  long previousTimeSec=0;
  char buf[40];

  oled_print(OC16, BLE_CONNECT1, BLE_CONNECT2, BLE_CONNECT3, ""); 

  sprintf(buf,"%s %02X:%02X:%02X:%02X:%02X:%02X",BLE_SERVER,macSTA[0],macSTA[1],macSTA[2],macSTA[3],macSTA[4],macSTA[5]);
  BLEDevice::init(buf);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY);     
  pTxCharacteristic->addDescriptor(new BLE2902());

  pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->addDescriptor(new BLE2902());
  
  //pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  
  DEBUGPRINTLN3("BLE service started");
  DEBUGPRINT3("connect with bluetooth to : ");DEBUGPRINTLN3(buf);
  
  // wait BLE connection
  previousTime = millis();
  previousTimeSec = previousTime/1000;
  while (!deviceConnected) {
      if ( millis() > (previousTimeSec*1000)+1000 ) {
        oled_print(OC16, BLE_CONNECT1, BLE_CONNECT2, BLE_CONNECT3, 
        (WAIT_TIME_FOR_BLE + previousTime - millis())/1000 + String("s ") + info ); 
        previousTimeSec = millis()/1000;
      }
      delay(10);
      timerWrite(wdt_timer, 0); //reset timer (feed watchdog)
      if ( millis() > previousTime + WAIT_TIME_FOR_BLE ) 
        ESP.restart(); //go_to_sleep (sleep_time, OLED_ON);
  }
  delay(1500); // needed before start !?
  oled_print(OC16, BLE_CONNECT1, BLE_CONNECT2, BLE_CONNECT4,"");
  DEBUGPRINT3("connected to : ");DEBUGPRINTLN3(buf);
}

// ==============================
uint8_t ble_settings_choice() {
  int ans = 0;
  
  previousTime = millis();
  write_to_ble_terminal (SETTING, LINE_FEED_ON);
  write_to_ble_terminal ("1 - ", LINE_FEED_OFF); write_to_ble_terminal (LORA, LINE_FEED_ON);
  write_to_ble_terminal ("2 - ", LINE_FEED_OFF); write_to_ble_terminal (WIFI, LINE_FEED_ON);
  write_to_ble_terminal ("3 - ", LINE_FEED_OFF); write_to_ble_terminal (RST_EEPROM, LINE_FEED_ON);
  write_to_ble_terminal ("4 - ", LINE_FEED_OFF); write_to_ble_terminal (WEB_HTACCESS, LINE_FEED_ON);
  do {
    write_to_ble_terminal (CHOICE, LINE_FEED_ON);
    read_ble_terminal (answer, sizeof(answer));
    ans = atoi(answer);
  } while ( ans < 1 || ans > 4 ); 
  return ans;
}

// ==============================
void ble_settings_wifi() {
  int ans = 0;
  
  write_to_ble_terminal (WIFI, LINE_FEED_ON);
  oled_print(OB24, SETTING, WIFI, "", "");
  do {
    //write_to_ble_terminal (SETTING, LINE_FEED_ON);
    write_to_ble_terminal ("1 - ", LINE_FEED_OFF); write_to_ble_terminal (LIST_NETWORK, LINE_FEED_ON);
    write_to_ble_terminal ("2 - ", LINE_FEED_OFF); write_to_ble_terminal (ADD_NETWORK, LINE_FEED_ON);
    write_to_ble_terminal ("3 - ", LINE_FEED_OFF); write_to_ble_terminal (DELETE_NETWORK, LINE_FEED_ON);
    write_to_ble_terminal ("4 - ", LINE_FEED_OFF); write_to_ble_terminal (SEARCH_NETWORK, LINE_FEED_ON);
    write_to_ble_terminal ("q - ", LINE_FEED_OFF); write_to_ble_terminal (EXIT, LINE_FEED_ON);
    do {
      write_to_ble_terminal (CHOICE, LINE_FEED_ON);
      read_ble_terminal (answer, sizeof(answer));
      if ( answer == "q" )
        ans = 0;
      else
        ans = atoi(answer);
    } while ( ans < 0 || ans > 4 ); 
    switch ( ans ) {
      case 1 :
        DEBUGPRINTLN3("BLE WiFi list");
        ble_settings_wifi_list();
        break;
      case 2 :
        DEBUGPRINTLN3("BLE WiFi add");
        ble_settings_wifi_add();
        break;
      case 3 :
        DEBUGPRINTLN3("BLE WiFi delete");
        ble_settings_wifi_del();
        break;
      case 4 :
        DEBUGPRINTLN3("BLE WiFi search");
        ble_settings_wifi_search();
        break;
      default :
        break;
    }
  } while ( ans != 0 ); 
}

// ==============================
void ble_settings_wifi_list() {
  int8_t  idx = 0 ;
  char    buf[45];
  
  if ( network_nb == 0 ) {
    write_to_ble_terminal (NO_KNOWN_NETWORK, LINE_FEED_ON);
  } else {
    write_to_ble_terminal (NETWORKS, LINE_FEED_ON);
    for (idx = 0; idx < network_nb; idx++) {
      sprintf(buf,"%d : %s", idx, ssids[idx], WiFi.RSSI(idx));
      write_to_ble_terminal(buf, LINE_FEED_ON);
    }
  }
  write_to_ble_terminal ("", LINE_FEED_ON);
}

// ==============================
void ble_settings_wifi_add() {
  int8_t  idx = 0 ;

  if ( network_nb >= NETWORK_NB_MAX ) {
    write_to_ble_terminal (TOO_MUCH_NETWORKS, LINE_FEED_ON);
  } else {
    write_to_ble_terminal (ENTER_SSID, LINE_FEED_ON);
    read_ble_terminal (ssids[network_nb], SSID_MAX_LEN);
    write_to_ble_terminal (ENTER_PASSWD, LINE_FEED_ON);
    read_ble_terminal (passwds[network_nb], PASSWD_MAX_LEN);
    if ( connect_to_network( network_nb ) ) {
      write_to_ble_terminal (CONNECT_NETWORKS_OK, LINE_FEED_OFF);
      write_to_ble_terminal (ssids[network_nb], LINE_FEED_ON);
      network_nb++;
      write_networks_to_eeprom();
    } else {
      DEBUGPRINTLN1("WiFi not connected");
      write_to_ble_terminal (CONNECT_NETWORKS_FAILED, LINE_FEED_OFF);
      write_to_ble_terminal (ssids[network_nb], LINE_FEED_ON);
    }
  }
  
  write_to_ble_terminal ("", LINE_FEED_ON);
}

// ==============================
void ble_settings_wifi_del() {
  int8_t  idx = 0 ;
  int8_t  id_del;
  int     ans = 0;

  ble_settings_wifi_list();
  write_to_ble_terminal (NETWORKS_TO_DEL_ID, LINE_FEED_ON);
  
  do {
    write_to_ble_terminal (CHOICE, LINE_FEED_ON);
    read_ble_terminal (answer, sizeof(answer));
    if ( answer == "q" )
      return;
    ans = atoi(answer);
  } while ( ans < 0 || ans >= network_nb ); 
  
  for( idx = ans+1 ; idx < network_nb ; idx++ ) {
    memcpy( ssids[idx-1], ssids[idx], sizeof(ssids[idx]));
    memcpy( passwds[idx-1], passwds[idx], sizeof(passwds[idx]));
  }
  memcpy( ssids[idx-1], "", sizeof(""));
  memcpy( passwds[idx-1], "", sizeof(""));
  network_nb--;

  ble_settings_wifi_list();

  write_networks_to_eeprom();
}

// ==============================
void ble_settings_wifi_search() {
  int8_t  idx = 0 ;
  int     ans = 0;
  int     scanned_network_nb = 0;
  char    str[32];
  char    buf[45];
  
  if ( network_nb >= NETWORK_NB_MAX ) {
    write_to_ble_terminal (TOO_MUCH_NETWORKS, LINE_FEED_ON);
  } else {
    // wifi scan
    scanned_network_nb = WiFi.scanNetworks();
    DEBUGPRINT3(scanned_network_nb); DEBUGPRINTLN3(" Networks found");
    
    if ( scanned_network_nb == 0 ) {
      write_to_ble_terminal (NO_NETWORK_FOUND, LINE_FEED_ON);
    } else {
      write_to_ble_terminal (NETWORKS, LINE_FEED_ON);
      for (idx = 0; idx < scanned_network_nb; idx++) {
        Serial.println(WiFi.SSID(idx));
        WiFi.SSID(idx).toCharArray(str, WiFi.SSID(idx).length());
        sprintf(buf,"%d : %s (%d dBm) %s", idx, str, WiFi.RSSI(idx), (WiFi.encryptionType(idx) == WIFI_AUTH_OPEN)?" ":" *");
        write_to_ble_terminal(buf, LINE_FEED_ON);
      }
      do {
        write_to_ble_terminal(CHOOSE_NETWORK, LINE_FEED_ON);
        read_ble_terminal (answer, sizeof(answer));
        if ( answer == "q" )
          return;
        ans = atoi(answer);
      } while ( ans < 0 || ans >= scanned_network_nb ); 
      strcpy( ssids[network_nb], WiFi.SSID(ans).c_str());
      write_to_ble_terminal (ENTER_PASSWD, LINE_FEED_ON);
      read_ble_terminal (passwds[network_nb], PASSWD_MAX_LEN);
      if ( connect_to_network( network_nb ) ) {
        write_to_ble_terminal (CONNECT_NETWORKS_OK, LINE_FEED_OFF);
        write_to_ble_terminal (ssids[network_nb], LINE_FEED_ON);
        network_nb++;
        write_networks_to_eeprom();
      } else {
        DEBUGPRINTLN1("WiFi not connected");
        write_to_ble_terminal (CONNECT_NETWORKS_FAILED, LINE_FEED_OFF);
        write_to_ble_terminal (ssids[network_nb], LINE_FEED_ON);
      }
    }
  }
  write_to_ble_terminal ("", LINE_FEED_ON);
}

// ==============================
void write_networks_to_eeprom() {
  int8_t  idx = 0 ;
  int address = LOCATION_NETWORK; // ssid 32 , passwd 64

  EEPROM.put(LOCATION_NETWORK_NB, network_nb); 

  for ( idx = 0 ; idx < NETWORK_NB_MAX ; idx++ ) {
    if ( idx < network_nb ) {
      EEPROM.writeString(address, ssids[idx]);
      address += SSID_MAX_LEN;
      EEPROM.writeString(address, passwds[idx]);
      address += PASSWD_MAX_LEN;
    } else {
      EEPROM.writeString(address, "");
      address += SSID_MAX_LEN;
      EEPROM.writeString(address, "");
      address += PASSWD_MAX_LEN;
    }
  }

  EEPROM.commit();
}

// ==============================
void ble_settings_web() {
  int8_t  idx = 0 ;
  int address = LOCATION_WEB_SERVER; // WEB_SERVER_MAX_LEN
  char buf[WEB_SERVER_MAX_LEN];
  
  write_to_ble_terminal (ENTER_WEB_SERVER, LINE_FEED_ON);  
  read_ble_terminal (buf, WEB_SERVER_MAX_LEN-strlen(http_str));
  sprintf(web_server,"%s%s",http_str,buf);
  write_to_ble_terminal (web_server, LINE_FEED_ON);  
  write_to_ble_terminal (ENTER_LOGIN, LINE_FEED_ON);
  read_ble_terminal (web_login, SSID_MAX_LEN);
  write_to_ble_terminal (ENTER_PASSWD, LINE_FEED_ON);
  read_ble_terminal (web_passwd, PASSWD_MAX_LEN);
  
  EEPROM.writeString(address, web_server);
  address = LOCATION_WEB_LOGIN; // login 32 , passwd 64
  EEPROM.writeString(address, web_login);
  address += SSID_MAX_LEN;
  EEPROM.writeString(address, web_passwd);
  EEPROM.commit();

  write_to_ble_terminal ("", LINE_FEED_ON);
}

// ==============================
int ble_select_nb_param (int nb, char *param, uint8_t nb_start, uint8_t nb_stop, uint8_t nb_step, char *units) {
  int     ans = 0;
  char    buf[45];

  do {
    sprintf(buf,"%s (%d %s)", param, nb, units);
    write_to_ble_terminal (buf, LINE_FEED_ON);
    sprintf(buf,"%s [%d-%d]", CHOICE, nb_start, nb_stop);
    write_to_ble_terminal (buf, LINE_FEED_ON);
    read_ble_terminal (answer, sizeof(answer));
    ans = atoi(answer);
  } while ( ans < nb_start || ans > nb_stop );
  sprintf(buf,"%s : %d %s\n", param, ans, units);
  write_to_ble_terminal (buf, LINE_FEED_ON);
  return ans;
}

// ==============================
uint8_t ble_select_freq_param_id (long *values, uint8_t value_nb, uint8_t value_id, char *freq_type) {
  uint8_t   i = 0;
  uint8_t   ans = 0;
  char      buf[45];
  float     coef = 1000000.0;
  char      units[] = "MHz";

  if ( freq_type == LR_SBW ) {
    units[0] = 'k';
    coef = 1000.0;
  }

  do {  
    sprintf(buf,"%s (%.1f %s)", freq_type, values[value_id]/coef, units);
    write_to_ble_terminal (buf, LINE_FEED_ON);
    for ( i = 0 ; i < value_nb ; i++ ) { 
      sprintf(buf,"%4d : %.1f %s     ", i, values[i]/coef, units);
      if ( i % 2 )
        write_to_ble_terminal (buf, LINE_FEED_ON);
      else
        write_to_ble_terminal (buf, LINE_FEED_OFF);
    }
    if ( i % 2 )
      write_to_ble_terminal ("", LINE_FEED_ON);
    sprintf(buf,"%s [%d-%d]", CHOICE, 0, value_nb );
    write_to_ble_terminal (buf, LINE_FEED_ON);
    read_ble_terminal (answer, sizeof(answer));
    ans = atoi(answer);
  } while ( ans < 0 || ans > value_nb );
  sprintf(buf,"%s : %.1f %s\n", freq_type, values[ans]/coef, units);
  write_to_ble_terminal (buf, LINE_FEED_ON);
  
  return ans;
}

// ==============================
void ble_settings_lora(struct LoRa_param_s  LR) {
  char      buf[45];
  
  write_to_ble_terminal (LORA, LINE_FEED_ON);
  LR.frequency_id = ble_select_freq_param_id (frequency_values,
    (sizeof(frequency_values)/sizeof(*frequency_values)), LR.frequency_id, LR_FREQ);
  LR.spreadingFactor = ble_select_nb_param (LR.spreadingFactor, LR_SF, 7, 12, 1, "");
  LR.signalBandwidth_id = ble_select_freq_param_id (signalBandwidth_values,
    (sizeof(signalBandwidth_values)/sizeof(*signalBandwidth_values)), LR.signalBandwidth_id, LR_SBW);
  LR.txPower = ble_select_nb_param (LR.txPower, LR_TXPOWER, 2, TX_MAX_POWER, 1, "dB");
 
  // save settings
  EEPROM.put(LOCATION_LORA_TXPOWER, LR.txPower);
  EEPROM.put(LOCATION_LORA_FREQUENCY, LR.frequency_id);
  EEPROM.put(LOCATION_LORA_SPREADINGFACTOR, LR.spreadingFactor);
  EEPROM.put(LOCATION_LORA_SIGNALBANDWIDTH, LR.signalBandwidth_id);
  //EEPROM.put(LOCATION_LORA_SYNCWORD, LoRa_param_1.syncWord);
  //EEPROM.put(LOCATION_LORA_OCP, );
  EEPROM.commit();    
  sprintf(buf,"%s %s", SETTING, SAVED);
  write_to_ble_terminal (buf, LINE_FEED_ON);
  write_to_ble_terminal (LOGOUT, LINE_FEED_ON);
}

// ==============================
void write_to_ble_terminal (char * text, bool line_feed) {
  
  pTxCharacteristic->setValue(text); 
  pTxCharacteristic->notify(); // send 'text' to the terminal
  DEBUGPRINT3(text);
  if ( line_feed ) {
    DEBUGPRINTLN3("");
    pTxCharacteristic->setValue("\n"); 
    pTxCharacteristic->notify();
  }
}

// ==============================
void read_ble_terminal ( char *param, uint8_t len_max) {
  
  std::string rxValue;
  uint8_t strTooLong = 0;
  uint8_t i = 0;
  uint8_t i_param = 0;
  
  for (i = 0; i < len_max ; i++) // reset param
      param[i] = '\0';
  
  do {
    strTooLong = 0;

    do 
      rxValue = pRxCharacteristic->getValue();
    while(rxValue.length() == 0); // wait answer  
    
    timerWrite(wdt_timer, 0); //reset timer (feed watchdog)  

    DEBUGPRINT3("Received Value : "); DEBUGPRINT3(rxValue.c_str()); DEBUGPRINT3("length: "); DEBUGPRINTLN3(rxValue.length());
    for (i = 0, i_param = 0; i < rxValue.length() ; i++) {
      if ( !iscntrl(rxValue[i]) ) { 
        if ( i_param < len_max ) { // remove CR, LF, ....
          param[i_param] = rxValue[i];
          i_param++;
        }
      }
      if ( i_param >= len_max ) {
        strTooLong++;
        i_param--;
      }
    }
    param[i_param] = '\0'; //'\0';
    DEBUGPRINT3("read ble string : ");DEBUGPRINT3(param);DEBUGPRINT3(", length: ");DEBUGPRINTLN3(strlen(param));
    DEBUGPRINTLN3("\n");
      
    if (strTooLong) { 
      write_to_ble_terminal(STRING_TO_LONG,LINE_FEED_ON); 
      pRxCharacteristic->setValue(""); //  reset RxCharacteristic value
    }
  } while ( strTooLong );
 
  pRxCharacteristic->setValue(""); // reset RxCharacteristic value
}
