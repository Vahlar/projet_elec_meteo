#include "esp_lora.h"

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
  LoRa.endPacket();
}

//*** stop LoRa ********************************************************
void LoRa_stop () {
  LoRa.sleep();
  LoRa.end();
}
