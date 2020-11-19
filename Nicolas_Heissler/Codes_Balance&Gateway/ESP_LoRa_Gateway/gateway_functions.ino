#include "gateway_functions.h"
#include "gateway_http.h"


//*** init_eeprom **********************************************************
void init_eeprom () {
  uint8_t idx = 0;
  int address = LOCATION_NETWORK; // ssid 32 , passwd 64;
  
  DEBUGPRINTLN0("************** init_eeprom **************");
  for( uint8_t i = LOCATION_MAC_ADDRESS ; i < LOCATION_MAC_ADDRESS+6 ; i++)
    EEPROM.put(i, mac_gateway[i]);

  EEPROM.put(LOCATION_LORA_TXPOWER, LoRa_param_1.txPower);
  EEPROM.put(LOCATION_LORA_FREQUENCY, LoRa_param_1.frequency_id);
  EEPROM.put(LOCATION_LORA_SPREADINGFACTOR, LoRa_param_1.spreadingFactor);
  EEPROM.put(LOCATION_LORA_SIGNALBANDWIDTH, LoRa_param_1.signalBandwidth_id);
  EEPROM.put(LOCATION_LORA_CODINGRATE, LoRa_param_1.codingRateDenominator);
  EEPROM.put(LOCATION_LORA_PREAMBLE, LoRa_param_1.preambleLength);
  //EEPROM.put(LOCATION_LORA_SYNCWORD, LoRa_param_1.syncWord);
  //EEPROM.put(LOCATION_LORA_OCP, );

  // reset Web server datas
  address = LOCATION_WEB_SERVER;
  EEPROM.writeString(address, "http://path.to.web/site/bees/");
  address = LOCATION_WEB_LOGIN;
  EEPROM.writeString(address, "");
  address += SSID_MAX_LEN;
  EEPROM.writeString(address, "");
  address += PASSWD_MAX_LEN;

  // reset networks datas
  EEPROM.put(LOCATION_NETWORK_NB,0); // network_nb 
  address = LOCATION_NETWORK;
  for ( idx = 0 ; idx < NETWORK_NB_MAX ; idx++ ) {
    EEPROM.writeString(address, "");
    address += SSID_MAX_LEN;
    EEPROM.writeString(address, "");
    address += PASSWD_MAX_LEN;
  }
  // initialyze with your networks
  address = LOCATION_NETWORK;
  EEPROM.put(LOCATION_NETWORK_NB,3); // network_nb 
  EEPROM.writeString(address, "");
  address += SSID_MAX_LEN;
  EEPROM.writeString(address, "");
  address += PASSWD_MAX_LEN;
  EEPROM.writeString(address, "");
  address += SSID_MAX_LEN;
  EEPROM.writeString(address, "");
  address += PASSWD_MAX_LEN;
  EEPROM.writeString(address, "");
  address += SSID_MAX_LEN;
  EEPROM.writeString(address, "");
  address += PASSWD_MAX_LEN;

  EEPROM.commit();

  init_eeprom_flag = true;
}

//*** eeprom **********************************************************
void read_eeprom() {
  uint8_t idx = 0;
  int address = LOCATION_NETWORK; // ssid 32 , passwd 64;
  String str;

  DEBUGPRINTLN3("### read_eeprom #############");
  DEBUGPRINTLN3("--- LoRa");
  EEPROM.get(LOCATION_LORA_TXPOWER, LoRa_param_1.txPower); //DEBUGPRINTLN3(LoRa_param_1.txPower);
  EEPROM.get(LOCATION_LORA_FREQUENCY, LoRa_param_1.frequency_id); //DEBUGPRINTLN3(LoRa_param_1.frequency);
  EEPROM.get(LOCATION_LORA_SPREADINGFACTOR, LoRa_param_1.spreadingFactor); //DEBUGPRINTLN3(LoRa_param_1.spreadingFactor);
  EEPROM.get(LOCATION_LORA_SIGNALBANDWIDTH, LoRa_param_1.signalBandwidth_id); //DEBUGPRINTLN3(LoRa_param_1.signalBandwidth);
  EEPROM.get(LOCATION_LORA_CODINGRATE, LoRa_param_1.codingRateDenominator); //DEBUGPRINTLN3(LoRa_param_1.codingRateDenominator);
  EEPROM.get(LOCATION_LORA_PREAMBLE, LoRa_param_1.preambleLength); //DEBUGPRINTLN3(LoRa_param_1.preambleLength);
  //EEPROM.get(LOCATION_LORA_SYNCWORD, LoRa_param_1.syncWord); DEBUGPRINTLN3(LoRa_param_1.syncWord);
  //EEPROM.get(LOCATION_LORA_OCP, ); DEBUGPRINTLN3();

  DEBUGPRINTLN3("--- Web server");
  address = LOCATION_WEB_SERVER;
  str = EEPROM.readString(address); 
  strcpy( web_server, str.c_str() ); DEBUGPRINT3(web_server);DEBUGPRINTLN3("");
  address = LOCATION_WEB_LOGIN;
  str = EEPROM.readString(address); 
  strcpy( web_login, str.c_str() ); DEBUGPRINT3(web_login);DEBUGPRINTLN3("");
  address += SSID_MAX_LEN;
  str = EEPROM.readString(address); 
  strcpy( web_passwd, str.c_str() ); //DEBUGPRINTLN3(web_passwd);  
  
  
  DEBUGPRINTLN3("--- WiFi");
  EEPROM.get(LOCATION_NETWORK_NB,network_nb); 
  DEBUGPRINT3("known network_nb"); DEBUGPRINTLN3(network_nb);
  address = LOCATION_NETWORK;
  for ( idx = 0 ; idx < NETWORK_NB_MAX ; idx++ ) {
    DEBUGPRINT3(idx);DEBUGPRINT3(":");
    str = EEPROM.readString(address); 
    strcpy( ssids[idx], str.c_str() ); DEBUGPRINT3(ssids[idx]);DEBUGPRINTLN3("");
    address += SSID_MAX_LEN;
    str = EEPROM.readString(address); 
    strcpy( passwds[idx], str.c_str() ); //DEBUGPRINTLN3(passwds[idx]);  
    address += PASSWD_MAX_LEN;
  }

  DEBUGPRINTLN3("################");  

}

//*** payload ********************************************************
uint8_t read_payload() {
  uint8_t idx = 0;
      
  DEBUGPRINT1("Payload : ");
  while (LoRa.available()) {
    if ( idx < PAYLOAD_LEN ) {
      payload[idx] = (uint8_t)LoRa.read();
      DEBUGPRINT1(payload[idx]); DEBUGPRINT1(".");
    } else {
      DEBUGPRINT1(LoRa.read()); DEBUGPRINT1("?");
    }
    idx++;     
  }
  DEBUGPRINT1(" - module = "); DEBUGPRINT1((char)payload[0]);
  DEBUGPRINT1(" - length = "); DEBUGPRINTLN1(idx);

  return idx;
}

//*** crc32 ********************************************************
uint8_t check_crc32(uint8_t lenght) {
  uint32_t checksum, checksum_sent;
  char     buf[25];
  
  checksum = CRC32::calculate(payload, lenght); // check_byte not in the crc calculation
  DEBUGPRINT3("local checksum = "); DEBUGPRINTLN3(checksum);
  checksum_sent = payload[payload_lenght] + (payload[payload_lenght + 1] * 256)
        + (payload[payload_lenght + 2] * 65536) + (payload[payload_lenght + 3] * 16777216);
  DEBUGPRINT3("sent  checksum = "); DEBUGPRINTLN3(checksum_sent);
  if ( checksum_sent == checksum ) {
    DEBUGPRINTLN3("CRC OK");
    return (uint8_t)1; // crc OK
  } else {
    DEBUGPRINTLN3("CRC KO");
    sprintf(buf,"%c %02X:%02X:%02X:%02X:%02X:%02X",payload[0], payload[1], payload[2], payload[3], payload[4], payload[5], payload[6]);
    oled_print(OC16, "CRC KO",buf , "RSSI " + (String)LoRa.packetRssi() + " dBm", "SNR " + (String)LoRa.packetSnr() + " dB");
    previousTime = millis();
    return (uint8_t)0; // crc KO
  }
}

//*** data ********************************************************
#if  DEBUGLEVEL > 1
void print_data(uint8_t *data) {
  uint8_t i = 0;
  
  Serial.print("--- Module : ");Serial.print((char)data[0]);Serial.println(" ------------");
  Serial.print("MAC : ");
  for ( i = 1 ; i < 7 ; i++ ) {
    Serial.print(data[i]);
    Serial.print(":");
  }
  Serial.println("");
  switch ( (char)data[0] ) {
    case 'S' :
      Serial.print("mass : ");
      ds.mass = (float)data[i++];
      ds.mass += (float)data[i++]/100.0;
      Serial.print(ds.mass); Serial.println(" kg");
      ds.v_bat = (float)data[i++]/10.0;
      Serial.print("v_bat : "); Serial.print(ds.v_bat); Serial.println(" V");
      ds.charg_bat = (uint8_t)data[i++];
      Serial.print("charg_bat : "); Serial.print(ds.charg_bat); Serial.println(" %");
      ds.action = (uint8_t)data[i++];
      Serial.print("action : "); Serial.println(ds.action);
      ds.temp = (float)data[i++];
      ds.temp += (float)data[i++]/100.0;
      ds.temp -= 100.0;
      Serial.print(ds.temp); Serial.println(" °C");
      ds.hygro = (uint8_t)data[i++];
      Serial.print("hygro : "); Serial.print(ds.hygro); Serial.println(" %");
      Serial.print("send   : "); Serial.println((uint8_t)(data[i]&0b00000111));
      Serial.print("errors : "); Serial.println((uint8_t)(data[i]&0b11111000));
      if ( data[i]&0b00001000 )
        Serial.println("         mass < 0");
      if ( data[i]&0b00010000 )
        Serial.println("         mass measure");
      if ( data[i]&0b00100000 )
        Serial.println("         temp measure");
      break;
    default : 
      Serial.print("unknown data format"); 
      break;
  }
  Serial.println("--------------------------");
}
#endif
