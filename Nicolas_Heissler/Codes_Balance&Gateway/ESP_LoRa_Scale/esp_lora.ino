/* weight scale for bee hives */
/* see : http://rucher.polytech.unice.fr/index.php
 * --------------------------------------------------
 * IMPORTANT : choose "Huge APP" in Tools -> Partition scheme
 * --------------------------------------------------
 * LICENCE
 * All rights reserved. 
 * This program and the accompanying materials are made available under the terms of the MIT License 
 * which accompanies this distribution, and is available at https://opensource.org/licenses/mit-license.php 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * --------------------------------------------------
 * Christian.PETER_at_univ-cotedazur.fr
 */

//*** start LoRa **********************************************************
void LoRa_start(struct LoRa_param_s  LR) {
  if (!LoRa.begin(frequency_values[LR.frequency_id])) {
    DEBUGPRINTLN2("LoRa init failed. Check your connections.");
    oled_print(OB24, "LoRa init", "KO", "connect° ?" , "");
    delay(3000);
    #if( MODULE == 'G')
    while(1);
    #else
    go_to_sleep (sleep_time_values[sleep_time_night_id], OLED_OFF);
    #endif
  }

  DEBUGPRINTLN2("LoRa Initial OK!");
  DEBUGPRINT3("LoRa Frequency: "); DEBUGPRINTLN3(frequency_values[LR.frequency_id]);
  DEBUGPRINT3("LoRa Spreading Factor: "); DEBUGPRINTLN3(LR.spreadingFactor);
  LoRa.setSpreadingFactor(LR.spreadingFactor);
  DEBUGPRINT3("LoRa Signal Bandwidth: "); DEBUGPRINTLN3(signalBandwidth_values[LR.signalBandwidth_id]);
  LoRa.setSignalBandwidth(signalBandwidth_values[LR.signalBandwidth_id]);
  DEBUGPRINT3("LoRa codingRateDenominator: "); DEBUGPRINTLN3(LR.codingRateDenominator);
  LoRa.setCodingRate4(LR.codingRateDenominator);
  DEBUGPRINT3("LoRa preambleLength: "); DEBUGPRINTLN3(LR.preambleLength);
  LoRa.setPreambleLength(LR.preambleLength);  
//  DEBUGPRINT3("LoRa syncWord: "); DEBUGPRINTLN3(LR.syncWord);
//  LoRa.setSyncWord(LR.syncWord);  
  DEBUGPRINT3("LoRa txPower: "); DEBUGPRINTLN3(LR.txPower);
  LoRa.setTxPower(LR.txPower);
  DEBUGPRINTLN3("LoRa enable CRC ");
  LoRa.enableCrc();  
}

//*** payload LoRa ********************************************************
void LoRa_send_payload(uint8_t *payload, uint8_t payload_len, uint8_t check_byte) {
  DEBUGPRINT2("LoRa send:");
  LoRa.beginPacket();
  for (uint8_t i = 0; i < payload_len; i++ ) {
    LoRa.write(payload[i]);
    DEBUGPRINT2(payload[i]);DEBUGPRINT2(".");
  }
  LoRa.write(check_byte);
  DEBUGPRINTLN2(check_byte);
  
  #ifdef CRC_32
  checksum = CRC32::calculate(payload, payload_len);
  DEBUGPRINT2("crc = "); DEBUGPRINT2(checksum); DEBUGPRINT2(" = "); 
  for (size_t j = 0; j < sizeof(checksum); j++) {
    // Convert to unsigned char* because a char is 1 byte in size. That is guaranteed by the standard.
    unsigned char b = *((unsigned char *)&checksum + j);
    LoRa.write(b); 
    DEBUGPRINT2(b); DEBUGPRINT2(".");
  }
  DEBUGPRINTLN2("");
  #endif
  
  LoRa.endPacket();
}

//*** stop LoRa ********************************************************
void LoRa_stop () {
  LoRa.sleep();
  LoRa.end();
}

/* settings with OLED */
/* 
//*** settings LoRa ********************************************************
void settings_lora(struct LoRa_param_s  LR) {

  LR.frequency_id = select_freq_param_id (frequency_values,
    (sizeof(frequency_values)/sizeof(*frequency_values)) -1, LR.frequency_id, FREQ_PARAM);
  LR.spreadingFactor = select_nb_param (LR.spreadingFactor, SF_PARAM, 7, 12, 1);
  LR.signalBandwidth_id = select_freq_param_id (signalBandwidth_values,
    (sizeof(signalBandwidth_values)/sizeof(*signalBandwidth_values)) -1, LR.signalBandwidth_id, SBW_PARAM);
  LR.txPower = select_nb_param (LR.txPower, TXPOWER_PARAM, 2, 17, 1);

//TXPOWER_PARAM, FREQ_PARAM, SF_PARAM, SBW_PARAM  LR.signalBandwidth_id
  
  // save settings
  EEPROM.put(LOCATION_LORA_TXPOWER, LR.txPower);
  EEPROM.put(LOCATION_LORA_FREQUENCY, LR.frequency_id);
  EEPROM.put(LOCATION_LORA_SPREADINGFACTOR, LR.spreadingFactor);
  EEPROM.put(LOCATION_LORA_SIGNALBANDWIDTH, LR.signalBandwidth_id);

  //EEPROM.put(LOCATION_LORA_SYNCWORD, LoRa_param_1.syncWord);
  //EEPROM.put(LOCATION_LORA_OCP, );
  EEPROM.commit();    
   
  oled_print(OC24, "", SETTING, SAVED, "");
  delay(AFF_TIME);
}

//*** settings freq ********************************************************
uint8_t select_freq_param_id (long *values, uint8_t value_nb, uint8_t value_id, enum param_type p_type) {
  uint8_t next = 0;
  uint8_t next_mem = 0;
  String  line1;
  String  line2;

  switch ( p_type ) {
  case FREQ_PARAM :
      line1 = LR_FREQ;
      line2 = (String)(values[value_id]/1000000.0);
      break;
  case SBW_PARAM :
      line1 = LR_SBW;
      line2 = (String)(values[value_id]/1000.0);
      break;
  default :
      break;
  }
  oled_print(OC16B2, line1, line2, CHOOSE, VALID);
  
  while ( !digitalRead(VALID_BTN) ) {
    next = digitalRead(NEXT_BTN);
      if ( next ) {
        if ( next_mem == 0 ) {
          next_mem = 1;
          value_id++;
          if ( value_id > value_nb ) 
            value_id = 0; 
          
          switch ( p_type ) {
            case FREQ_PARAM :
                line1 = LR_FREQ;
                line2 = (String)(values[value_id]/1000000.0);
                break;
            case SBW_PARAM :
                line1 = LR_SBW;
                line2 = (String)(values[value_id]/1000.0);
                break;
            default :
                line1 = "error";
                line2 = "error";
                break;
          }Serial.println(line2);Serial.println(value_id);

          oled_print(OC16B2, line1, line2, CHOOSE, VALID);
        }
      } else {
        next_mem = 0;
      }
  }
  while ( digitalRead(VALID_BTN) );
  return value_id;
}

*/
