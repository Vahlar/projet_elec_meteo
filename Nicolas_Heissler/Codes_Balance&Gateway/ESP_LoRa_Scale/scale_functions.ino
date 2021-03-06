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

//*** ADC **********************************************************
float get_voltage(adc1_channel_t channel,adc_atten_t channel_atten, int measure_nb, float divider_coef) {
    long sum = 0;                  // sum of samples taken
    float voltage = 0.0;           // calculated voltage
    float full_scale_voltage = 0.0;

    adc1_config_channel_atten(channel,channel_atten);

    switch (channel_atten) {
      case ADC_ATTEN_DB_11 : full_scale_voltage = 3.9; break; // 150 to 2450 mV
      case ADC_ATTEN_DB_6  : full_scale_voltage = 2.2; break; // 150 to 1750 mV
      case ADC_ATTEN_DB_2_5: full_scale_voltage = 1.5; break; // 100 and 1250 mV
      case ADC_ATTEN_DB_0  : full_scale_voltage = 1.1; break; // 100 and 950 mV
      default              : full_scale_voltage = 1.1; break;
    }
    
    for (int i = 0; i < measure_nb; i++) {
        sum += adc1_get_voltage(channel);
        delayMicroseconds(1000);
    }
    
    // calculate the voltage
    voltage = (float)sum / (float)measure_nb;
    //Serial.print("adc1_get_voltage raw ");Serial.println(voltage);
    voltage = (voltage * full_scale_voltage) / 4095.0; 
    //Serial.print("adc1_get_voltage voltage pin ");Serial.println(voltage);
    // use if added divider circuit
    voltage = voltage / divider_coef;
    //Serial.print("adc1_get_voltage voltage batt ");Serial.println(voltage);
    //round value by two precision
    voltage = roundf(voltage * 100) / 100;
    return voltage;
}

//*** init init_eeprom **********************************************************
void init_eeprom () {
  DEBUGPRINTLN0("************** init_eeprom **************");
  for( uint8_t i = LOCATION_MAC_ADDRESS ; i < LOCATION_MAC_ADDRESS+6 ; i++)
    EEPROM.put(i, ds.macSTA[i]);

  EEPROM.put(LOCATION_LORA_TXPOWER, LoRa_param_1.txPower);
  EEPROM.put(LOCATION_LORA_FREQUENCY, LoRa_param_1.frequency_id);
  EEPROM.put(LOCATION_LORA_SPREADINGFACTOR, LoRa_param_1.spreadingFactor);
  EEPROM.put(LOCATION_LORA_SIGNALBANDWIDTH, LoRa_param_1.signalBandwidth_id);
  EEPROM.put(LOCATION_LORA_CODINGRATE, LoRa_param_1.codingRateDenominator);
  EEPROM.put(LOCATION_LORA_PREAMBLE, LoRa_param_1.preambleLength);
  //EEPROM.put(LOCATION_LORA_SYNCWORD, LoRa_param_1.syncWord);
  //EEPROM.put(LOCATION_LORA_OCP, );

  EEPROM.put(LOCATION_SLEEP_TIME_DAY, sleep_time_day_id);
  EEPROM.put(LOCATION_SLEEP_TIME_NIGHT, sleep_time_night_id);
  EEPROM.put(LOCATION_SLEEP_TIME_FOR_VISIT, sleep_time_for_visit_id);
  EEPROM.put(LOCATION_SEND_MAX_TRIES, send_max_tries);

  EEPROM.put(LOCATION_CALIBRATION_TEMP,calibration_temp); 
  EEPROM.put(LOCATION_TEMP_FACTOR,temp_factor); 
  EEPROM.put(LOCATION_SCALEA_CAL_FACTOR,0); // calibrationFactor[0]); 
  EEPROM.put(LOCATION_SCALEA_ZERO,0); //  zeroOffset[0]);
  EEPROM.put(LOCATION_SCALEB_CAL_FACTOR,0); //  calibrationFactor[1]); 
  EEPROM.put(LOCATION_SCALEB_ZERO,0); //  zeroOffset[1]);
  EEPROM.put(LOCATION_SCALEC_CAL_FACTOR,0); //  calibrationFactor[2]); 
  EEPROM.put(LOCATION_SCALEC_ZERO,0); //  zeroOffset[2]);
  EEPROM.put(LOCATION_SCALED_CAL_FACTOR,0); //  calibrationFactor[3]); 
  EEPROM.put(LOCATION_SCALED_ZERO,0); //  zeroOffset[3]);
  EEPROM.put(LOCATION_KNOWN_WEIGHT,0); //  know_weight);
  EEPROM.put(LOCATION_MASS_OFFSET,0); //  massOffset);
  EEPROM.put(LOCATION_SENSOR_NB,0); //  sensor_nb);

  // temperature 18B20 rom address
  for( uint8_t i = LOCATION_18B20_ROM ; i < LOCATION_18B20_ROM+8 ; i++)
    EEPROM.put(i, 0);

  EEPROM.commit();

  init_eeprom_flag = true;
}

//*** eeprom **********************************************************
void read_eeprom() {
  //DEBUGPRINTLN3("### read_eeprom #############");
  EEPROM.get(LOCATION_SLEEP_TIME_DAY, sleep_time_day_id); //DEBUGPRINTLN3(sleep_time_day_id);
  EEPROM.get(LOCATION_SLEEP_TIME_NIGHT, sleep_time_night_id); //DEBUGPRINTLN3(sleep_time_night_id);
  EEPROM.get(LOCATION_SLEEP_TIME_FOR_VISIT, sleep_time_for_visit_id); //DEBUGPRINTLN3(sleep_time_for_visit_id);
  EEPROM.get(LOCATION_SEND_MAX_TRIES, send_max_tries); //DEBUGPRINTLN3(send_max_tries);
  //DEBUGPRINTLN3("--- LoRa");
  EEPROM.get(LOCATION_LORA_TXPOWER, LoRa_param_1.txPower); //DEBUGPRINTLN3(LoRa_param_1.txPower);
  EEPROM.get(LOCATION_LORA_FREQUENCY, LoRa_param_1.frequency_id); //DEBUGPRINTLN3(LoRa_param_1.frequency);
  EEPROM.get(LOCATION_LORA_SPREADINGFACTOR, LoRa_param_1.spreadingFactor); //DEBUGPRINTLN3(LoRa_param_1.spreadingFactor);
  EEPROM.get(LOCATION_LORA_SIGNALBANDWIDTH, LoRa_param_1.signalBandwidth_id); //DEBUGPRINTLN3(LoRa_param_1.signalBandwidth);
  EEPROM.get(LOCATION_LORA_CODINGRATE, LoRa_param_1.codingRateDenominator); //DEBUGPRINTLN3(LoRa_param_1.codingRateDenominator);
  EEPROM.get(LOCATION_LORA_PREAMBLE, LoRa_param_1.preambleLength); //DEBUGPRINTLN3(LoRa_param_1.preambleLength);
  //EEPROM.get(LOCATION_LORA_SYNCWORD, LoRa_param_1.syncWord); DEBUGPRINTLN3(LoRa_param_1.syncWord);
  //EEPROM.get(LOCATION_LORA_OCP, ); DEBUGPRINTLN3();
  //DEBUGPRINTLN3("--- scale");
  EEPROM.get(LOCATION_CALIBRATION_TEMP,calibration_temp); 
  EEPROM.get(LOCATION_TEMP_FACTOR,temp_factor); //DEBUGPRINTLN3(temp_factor);
  EEPROM.get(LOCATION_SCALEA_CAL_FACTOR, calibrationFactor[0]); //DEBUGPRINTLN3(calibrationFactor[0]);
  EEPROM.get(LOCATION_SCALEA_ZERO, zeroOffset[0]); //DEBUGPRINTLN3(zeroOffset[0]);
  EEPROM.get(LOCATION_SCALEB_CAL_FACTOR, calibrationFactor[1]);  //DEBUGPRINTLN3(calibrationFactor[1]);
  EEPROM.get(LOCATION_SCALEB_ZERO, zeroOffset[1]); //DEBUGPRINTLN3(zeroOffset[1]);
  EEPROM.get(LOCATION_SCALEC_CAL_FACTOR, calibrationFactor[2]);  //DEBUGPRINTLN3(calibrationFactor[2]);
  EEPROM.get(LOCATION_SCALEC_ZERO, zeroOffset[2]); //DEBUGPRINTLN3(zeroOffset[2]);
  EEPROM.get(LOCATION_SCALED_CAL_FACTOR, calibrationFactor[3]);  //DEBUGPRINTLN3(calibrationFactor[3]);
  EEPROM.get(LOCATION_SCALED_ZERO, zeroOffset[3]); //DEBUGPRINTLN3(zeroOffset[3]);
  EEPROM.get(LOCATION_KNOWN_WEIGHT, know_weight);
  EEPROM.get(LOCATION_MASS_OFFSET, massOffset);
  EEPROM.get(LOCATION_SENSOR_NB, sensor_nb); //DEBUGPRINTLN3(sensor_nb);

  //DEBUGPRINTLN3("################");  

}

//*** action **********************************************************
uint8_t get_action(){
  long actionTime;
  uint8_t next = 0;
  uint8_t next_mem = 0;
  uint8_t action = 0;
  
  oled_print(OB24, "", ACTION_SELECT1, ACTION_SELECT2, "");
  actionTime = millis();
  while ( !digitalRead(VALID_BTN) ) {
    if ( millis() > actionTime + WAIT_TIME_FOR_NEXT_BTN )
      return 0; // no action selected
    next = digitalRead(NEXT_BTN);
    if ( next ) {
      timerWrite(wdt_timer, 0); //reset timer (feed watchdog)
      if ( next_mem == 0 ) {
        next_mem = 1;
        action++;
        actionTime = millis();
        if ( action >= sizeof(actions_order) ) { 
          action = 0; 
        }
        oled_print(OB24, "", action_text[actions_order[action]][0], action_text[actions_order[action]][1], "");
      }
    } else {
      next_mem = 0;
    }
  } 
  
  return actions_order[action];
}

//*** charge battery **********************************************************
uint8_t get_charg_bat() {
  return 255;
}


/* settings with OLED */
/* 
//*** settings_general ********************************************************
void settings_general () {
  
  sleep_time_day_id       = select_time_param_id (sleep_time_day_id,SLEEP_TIME_DAY_PARAM)  ;
  sleep_time_night_id     = select_time_param_id (sleep_time_night_id,SLEEP_TIME_NIGHT_PARAM)  ;
  sleep_time_for_visit_id = select_time_param_id (sleep_time_for_visit_id,SLEEP_TIME_FOR_VISIT_PARAM)  ;
  
  send_max_tries = (uint8_t)select_nb_param (send_max_tries, SEND_MAX_TRIES_PARAM, 1, SEND_MAX_TRIES_MAXI, 1);
  
  // save settings
  EEPROM.put(LOCATION_SLEEP_TIME_DAY, sleep_time_day_id);
  EEPROM.put(LOCATION_SLEEP_TIME_NIGHT, sleep_time_night_id);
  EEPROM.put(LOCATION_SLEEP_TIME_FOR_VISIT, sleep_time_for_visit_id);
  EEPROM.put(LOCATION_SEND_MAX_TRIES, send_max_tries);
  EEPROM.commit();    
   
  oled_print(OC24, "", SETTING, SAVED, "");
  delay(AFF_TIME);
}

//*** settings nb ********************************************************
int select_nb_param (uint8_t nb, enum param_type p_type, uint8_t nb_start, uint8_t nb_stop, uint8_t nb_step) {
  uint8_t next = 0;
  uint8_t next_mem = 0;
  String  line;
  String  line2;

  switch ( p_type ) {
  case SEND_MAX_TRIES_PARAM :
      line  = SEND_MAX_TRIES;
      line2 = SEND_MAX_TRIES1;
      break;
  case TXPOWER_PARAM :
      line = LR_TXPOWER;
      line2 = "";
      break;
  case SF_PARAM :
      line = LR_SF;
      line2 = "";
      break;
  default :
      line = "bad time ";
      line2 = "";
      break;
  }
  oled_print(OC16B2, line, line2 + (String)nb, CHOOSE, VALID);
  
  while ( !digitalRead(VALID_BTN) ) {
    next = digitalRead(NEXT_BTN);
      if ( next ) {
        if ( next_mem == 0 ) {
          next_mem = 1;
          nb++;
          if ( nb > nb_stop ) 
            nb = nb_start; 
          oled_print(OC16B2, line, line2 + (String)nb, CHOOSE, VALID);
        }
      } else {
        next_mem = 0;
      }
  }
  while ( digitalRead(VALID_BTN) );
  return nb;
}

//*** settings time ********************************************************
uint8_t select_time_param_id (uint8_t value_id, enum param_type p_type) {
  uint8_t value_nb = (sizeof(sleep_time_values)/sizeof(*sleep_time_values)) -1 ;
  uint8_t next = 0;
  uint8_t next_mem = 0;
  String  line;
  String  line2;

  switch ( p_type ) {
  case SLEEP_TIME_DAY_PARAM :
      line = SLEEP_TIME_DAY;
      break;
  case SLEEP_TIME_NIGHT_PARAM :
      line = SLEEP_TIME_NIGHT;
      break;
  case SLEEP_TIME_FOR_VISIT_PARAM :
      line = SLEEP_TIME_VISIT;
      break;
  default :
      line = "bad time ";
      break;
  }
  if ( sleep_time_values[value_id] < 120 ) 
    line2 = line + (String)sleep_time_values[value_id] + (String)" '" ;
  else 
    line2 = line + (String)(sleep_time_values[value_id]/60) + (String)" h" ;
  oled_print(OC16B2, SLEEP_TIME, line2, CHOOSE, VALID);
  
  while ( !digitalRead(VALID_BTN) ) {
    next = digitalRead(NEXT_BTN);
      if ( next ) {
        if ( next_mem == 0 ) {
          next_mem = 1;
          value_id++;
          if ( value_id > value_nb ) 
            value_id = 0; 
          if ( sleep_time_values[value_id] < 120 ) 
            line2 = line + (String)sleep_time_values[value_id] + (String)" '" ;
          else 
            line2 = line + (String)(sleep_time_values[value_id]/60) + (String)" h" ;
          oled_print(OC16B2, SLEEP_TIME, line2, CHOOSE, VALID);
        }
      } else {
        next_mem = 0;
      }
  }
  while ( digitalRead(VALID_BTN) );
  return value_id;
}

*/
