/* gateway for bee hives */
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


// ***********************************************************************
// ** adapt to your config
// ** change default LoRa parameters in esp_lora.h
// ** change web_url, http_str & url(s) in gateway_http.h (depends on your web site)
// choose language
#define LANGUAGE 'F' // 
#define TTGO // to use with TTGO ESP32 LoRa OLED - no other choice at this time
// ***********************************************************************
const char version[] = " 1.31";
#define MODULE 'G' // G:gateway, S:scale (weight), I:internal, W:weather

// debug
uint8_t verbose = 1;  // send email when reboot and wifi connect, click prog button at startup to valid verbose mode
#define DEBUGLEVEL 3 // -1 (mute) to 3 (verbose)
#include <DebugUtils.h>

uint8_t mac_gateway[6];

// time
enum param_type {TXPOWER_PARAM, FREQ_PARAM, SF_PARAM, SBW_PARAM};
#define FREQ_COEF 3 // to keep the same delay with rtc_clk_cpu_freq modification (RTC_CPU_FREQ_80M -> 240/80 = 3)
#define AFF_TIME  4000/FREQ_COEF  // time the data are visible on the OLED display
#define WAIT_TIME_FOR_CONFIG  3000/FREQ_COEF  // milli seconds
#define WAIT_TIME_FOR_NEXT_BTN          10000/FREQ_COEF // milliseconds
long    previousTime = 0;
long    currentTime = 0;

// sleep - wake up
//#define BUTTON_PIN_BITMASK 0x000000004 // 2^2 in hex VALID input
uint16_t sleep_time = 15; // minutes
#define uS_TO_MN_FACTOR 60000000ULL  /* Conversion factor for micro seconds to minutes */

// datas
// scale
struct datas { // 255 <=> error
  uint8_t macSTA[6];
  float   mass = 255;         // 0 to 200 000 g,
  float   v_bat = 255;        // 0 to 16 V
  uint8_t charg_bat = 255;    // 0 to 100%
  uint8_t action; // 0:no_action,1:Sirup50/50,2:Sirup70/30,3:Candy,4:materiel,5:Visit,6:measure only,7:sleep,8:infos bat,9:version
  float   temp  = 255;        // -50 to +80 °C
  uint8_t hygro = 255;        //  0 to 100%
} ds;

#define ENR_NB 10
#define LEN1   15
#define LEN2   25
#define LENT   (LEN1+LEN1+LEN2+LEN2)
struct enrs {
  char mess[25] = "";
  char object[25] = "";
  char rssi[15] = "";
  char snr[15] = "";
};
struct enrs enr[ENR_NB];

#define PAYLOAD_LEN 50 
uint8_t payload[PAYLOAD_LEN];
uint8_t payload_lenght; // payload with check_byte

// GPIO
#define MEASURE 13
#define VALID_BTN 2   
#define NEXT_BTN  4   
#define PROG_BTN 0  
uint8_t memBtn = 0;
uint8_t display_nb = 0;  

#ifdef TTGO
#define LED_G 2 //25 heltec - 2 TTGO  onboard
#else
#define LED_R 33
#define LED_G 25
#endif

#ifdef TTGO
// LoRa pins
  #define SS 18   // 5  - 18 TTGO
  #define RST 14
  #define DI0 26
  // SPI pins
  #define SCK 5     // 18 - 5 TTGO
  #define MISO 19
  #define MOSI 27   // 23 - 27 TTGO
  #define CS 18     //5 - 18 TTGO
  // I2C pins OLED
  #define SCL  15
  #define SDA  4
  #define OLED_RST  16
#else
  // LoRa pins
  #define SS 5   // 5  - 18 TTGO
  #define RST 14
  #define DI0 26
  // SPI pins
  #define SCK 18     // 18 - 5 TTGO
  #define MISO 19
  #define MOSI 23   // 23 - 27 TTGO
  #define CS 5     //5 - 18 TTGO
  // I2C pins
  #define SDA  21
  #define SCL  22
#endif



#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <soc/rtc.h>  // for rtc_clk_cpu_freq_set
#include <rom/rtc.h>
#include "EEPROM.h"
#include <HTTPClient.h>

#include "gateway_language.h"
#include "esp_lora.h"
#include "global_functions.h"
#include "gateway_ble.h"
#include "gateway_functions.h"
#include "gateway_wifi.h"

// eeprom
bool init_eeprom_flag = false;

// OLED
// need to modify ./libraries/...../OLEDDisplayFonts.h
#ifdef TTGO
  #include "SSD1306.h"
  SSD1306 display(0x3c, SDA, SCL);
#else
  #include "SH1106.h"  // or #include "SSD1306.h"
  SH1106 display(0x3c, SDA, SCL); // or SSD1306 display(0x3c, SDA, SCL);
#endif
enum oled_state {OLED_OFF, OLED_ON};

// LoRa
//#define ACKNOWLEDGE_LENGTH 8
int     packetSize;
//bool    ack_OK = false;
//bool    myMACaddress = true;

// CRC 
#include <CRC32.h>
CRC32 crc;

// === Watchdog ==============================
#define WDT_TIME_LIMIT 600000  // 100000 => 20s
hw_timer_t *wdt_timer = NULL; //watchdog timer
void IRAM_ATTR resetModule() {
  ets_printf("Watchdog reboot\n");
  esp_restart();
}

// ***********************************************************************
// ** setup
// ***********************************************************************
void setup() {
  uint8_t i = 0 ;
  uint8_t ui8_tmp;
  uint8_t next = 0;
  uint8_t next_mem = 0;
  uint8_t valid = 0;
  uint8_t valid_mem = 0;
  
  // For consumption
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  //RTC_CPU_FREQ_XTAL = 0,      //!< Main XTAL frequency
  //RTC_CPU_FREQ_80M = 1,       //!< 80 MHz
  //RTC_CPU_FREQ_160M = 2,      //!< 160 MHz
  //RTC_CPU_FREQ_240M = 3,      //!< 240 MHz
  //RTC_CPU_FREQ_2M = 4,        //!< 2 MHz ?????????????????????????????
  //(esp_bluedroid_disable(), esp_bt_controller_disable(), esp_wifi_stop())

  // Watchdog
  wdt_timer = timerBegin(0, 80 * 1000, true); // ???? timer 0, div 80*1000 - clock is 80MHZ so every tick will take 1 millisecond
  timerAttachInterrupt(wdt_timer, &resetModule, true);
  timerAlarmWrite(wdt_timer, WDT_TIME_LIMIT, false);
  timerAlarmEnable(wdt_timer); //enable interrupt
  timerWrite(wdt_timer, 0); //reset timer (feed watchdog)

  #ifdef TTGO
  pinMode(LED_G, OUTPUT);
  pinMode(OLED_RST, OUTPUT);
  #else
  pinMode(MEASURE, OUTPUT);
  digitalWrite(MEASURE, LOW); // Peripherals power off
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  #endif

  /* We set our ESP32 to wake up for an external trigger.
    There are two types for ESP32, ext0 and ext1 .
    ext0 uses RTC_IO to wakeup thus requires RTC peripherals to be on while
    ext1 uses RTC Controller so doesnt need peripherals to be powered on.
    Note that using internal pullups/pulldowns also requires RTC peripherals to be turned on. */
  // configure the wake up source
  //esp_sleep_enable_ext0_wakeup((gpio_num_t)VALID_BTN, 1); //1 = High, 0 = Low
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  Serial.begin(115200);
  while (!Serial) {;}

  DEBUGPRINT0(INIT_MSG1); DEBUGPRINT0(" "); DEBUGPRINT0(INIT_MSG2); DEBUGPRINTLN0(" Ver. " + (String)version); 
  
  DEBUGPRINT0("DEBUG LEVEL "); DEBUGPRINT3(" 3 "); DEBUGPRINT2(" 2 "); DEBUGPRINT1(" 1 "); DEBUGPRINTLN0(" 0 ");

  //get_macAdress
  esp_efuse_read_mac(mac_gateway);
  Serial.println("== mac STA ======");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X = ",
                mac_gateway[0], mac_gateway[1], mac_gateway[2], mac_gateway[3], mac_gateway[4], mac_gateway[5]);
  Serial.printf("%d:%d:%d:%d:%d:%d\n",
                mac_gateway[0], mac_gateway[1], mac_gateway[2], mac_gateway[3], mac_gateway[4], mac_gateway[5]);
  Serial.println("=================");

  // Peripherals power on
  #ifndef TTGO
  digitalWrite(MEASURE, HIGH); 
  #endif
  
  SPI.begin(SCK, MISO, MOSI, CS);
  LoRa.setPins(SS, RST, DI0);
  Wire.begin(SDA, SCL);
  Wire.setClock(400000); //increase I2C clock speed to 400kHz
  
  // eeprom
  if (!EEPROM.begin(EEPROM_SIZE)) {
    DEBUGPRINTLN0("Failed to initialise EEPROM");
    DEBUGPRINT0("size : "); DEBUGPRINTLN0(EEPROM_SIZE);
    oled_print(OB24, "INIT", "EEPROM", "KO", "");
    while(1);
  } else {
    DEBUGPRINT0("EEPROM size : "); DEBUGPRINTLN0(EEPROM_SIZE);
  }

  // reset ?
  #if  DEBUGLEVEL > 0
  switch (rtc_get_reset_reason(0) ) {
    case POWERON_RESET :
      DEBUGPRINTLN0("reset_reason : POWERON_RESET");
      break;
    case SW_CPU_RESET : // watchdog reset
      DEBUGPRINTLN0("reset_reason : SW_CPU_RESET");
      break;
    default :
      break;
  }
  Serial.println("=================");
  #endif
      
  // eeprom - if the 6 first byte are the mac address then the eeprom has been initialized
  init_eeprom_flag = true;
  for( i = LOCATION_MAC_ADDRESS ; i < LOCATION_MAC_ADDRESS+6 ; i++) {
    EEPROM.get(i, ui8_tmp);
    if ( ui8_tmp != mac_gateway[i] ) {
      init_eeprom_flag = false;
      break;
    }
  }
  if ( !init_eeprom_flag ) {
    init_eeprom();
  }
  read_eeprom();

  // init / settings
  if ( !network_nb ) { // no wifi network defined
    DEBUGPRINTLN2("SETTINGS WiFi");
    create_ble_uart (mac_gateway, "(WiFi ?)");
    ble_settings_wifi();
    write_to_ble_terminal (NO_WIFI3, LINE_FEED_ON); // reboot
    oled_print(OC24, NO_WIFI3, "", "", "");
    delay(AFF_TIME);
    ESP.restart();
  }
  previousTime = millis();
  oled_print(OB24, INIT_MSG1, INIT_MSG2, "Ver. " + (String)version , "");
  while ( millis() < previousTime + AFF_TIME ) {
    if ( !digitalRead(PROG_BTN) )
      verbose++;
  }
  oled_print(OB16, BLE_INVIT1, BLE_INVIT2, BLE_INVIT3 , SETTING);
  previousTime = millis();
  i = 0;
  while ( millis() < WAIT_TIME_FOR_CONFIG + previousTime && i == 0 ) {
    if ( !digitalRead(PROG_BTN) ) {   // if PROG_BTN during init message => enter setting mode
      DEBUGPRINTLN2("SETTINGS");
      create_ble_uart (mac_gateway, "");   
      i = ble_settings_choice();
      switch ( i ) {   
          case 0 : break;         
          case 1 : DEBUGPRINTLN2("go to LoRa settings");
                   oled_print(OB24, SETTING, "LORA", "", "");
                   ble_settings_lora(LoRa_param_1);
                   break;
          case 2 : DEBUGPRINTLN2("go to WiFi");
                   oled_print(OC24, SETTING, "WiFi", "", "");
                   ble_settings_wifi();
                   break;
          case 3 : DEBUGPRINTLN2("INIT EEPROM");
                   init_eeprom();
                   oled_print(OB24, INIT_SYS1, INIT_SYS2, INIT_SYS3, "");
                   delay(AFF_TIME);
                   break;
          case 4 : DEBUGPRINTLN2("go to web settings");
                   ble_settings_web();
                   oled_print(OC24, SETTING, "Web", "htaccess", "");
                   break;
          default: DEBUGPRINTLN2("bad choice");
                   break;
      }
      write_to_ble_terminal (NO_WIFI3, LINE_FEED_ON); // reboot
      oled_print(OC24, NO_WIFI3, "", "", "");
      delay(AFF_TIME);
      ESP.restart();
    }
  }
  Serial.println("=================");
  
  // blink at startup
  flash_led(LED_G, 200, 0, 1); // make also a delay to stabilize power on peripherals

  // LoRa
  LoRa_start(LoRa_param_1);
  DEBUGPRINTLN0("LoRa started");
  oled_print(OB16, "LoRa",
              "f:" + String(frequency_values[LoRa_param_1.frequency_id]/1000000.0) + "MHz", 
              "sf:" + String(LoRa_param_1.spreadingFactor),
              "bw:" + String(signalBandwidth_values[LoRa_param_1.signalBandwidth_id]/1000.0) + "kHz");
  delay(AFF_TIME);
  
  // wifi
  DEBUGPRINTLN0("========================");
  DEBUGPRINTLN0("connect wifi (scan)");
  connectWifi();
  
  display.displayOff();
  
  if (verbose)
    send_reboot_mess();

  DEBUGPRINTLN1("#####################################");
}

// ***********************************************************************
// ** loop
// ***********************************************************************
void loop() {
//uint8_t i=0;

  timerWrite(wdt_timer, 0); //reset timer (feed watchdog)

  if ( millis() > previousTime + (AFF_TIME * 3) ) {
    display_nb = 0;
    display.displayOff();
  }
  
  // wait packet ==========================
  packetSize = LoRa.parsePacket();
  
  if ( packetSize ) {
    DEBUGPRINT1("Received packet - size : ");DEBUGPRINTLN1(packetSize);
    
    switch ( packetSize ) { // test the lenght of payload, then read copy it in payload[]
      case 16 : // module S without crc32
        payload_lenght = read_payload(); // payload with check_byte
        //delay(50); // time needed for the sender to get the acknowledge
        LoRa_send_payload(payload, 7, (uint8_t)1); //send module bit + sender mac adress + error bit (=1 crc OK)
        break;
      case 20 : // module S with crc32
        payload_lenght = read_payload() - 4 ; // payload with check_byte, without crc32
        //delay(50); // time needed for the sender to get the acknowledge
        LoRa_send_payload(payload, 7, check_crc32(payload_lenght-1) ); //send module bit + sender mac adress + error bit (=0 crc KO)       
                                              // (payload_lenght-1) because the check_byte not in the crc calculation
        break;
      default : // unknown lenght, not for me !
        LoRa.readString();  // clear the buffer
        DEBUGPRINTLN1("Not the good lenght !");
        payload[0] = '?'; payload[1] = '\0';
        payload_lenght = 2;
        break;
    }
    // for me
    if ( payload_lenght > 2 ) {
      if ( WiFi.status() != WL_CONNECTED )
        connectWifi();

      #if  DEBUGLEVEL > 1
      print_data(payload);
      #endif
  
      write_data_to_DB(build_request());
  
     // print RSSI of packet
    DEBUGPRINT1("-> with LoRa RSSI="); DEBUGPRINT1(LoRa.packetRssi());
    DEBUGPRINT1(" dBm & SNR="); DEBUGPRINT1(LoRa.packetSnr()); DEBUGPRINTLN1(" dB");
    DEBUGPRINTLN1("#####################################");

    //    switch ( (char)payload[0] ) {
    //      case 'S' :
    //        Serial.print("mass : ");
    //        break;
    //      case 'W' :
    //        Serial.print("wheather : ");
    //        break;
    //      case 'I' :
    //        Serial.print("intern : ");
    //        break;
    //      case 'G' :
    //        Serial.print("Gateway !? should not happen");
    //        break;
    //      default : 
    //        Serial.print("unknown data format"); 
    //        break;
    //    }
    }
  }

  // display info ==========================
  if ( digitalRead(PROG_BTN) ) {
    memBtn = 0;
  } else if ( !memBtn ) {
    memBtn = 1;
    display_nb++;
    //Serial.println(display_nb);
    previousTime = millis();
    switch ( display_nb ) {
      case 1 : oled_print(OB24, INIT_MSG1, INIT_MSG2, "Ver. " + (String)version , ""); 
              break;
      case 2 : oled_print(OB16, "LoRa",
              "f:" + String(frequency_values[LoRa_param_1.frequency_id]/1000000.0) + "MHz", 
              "sf:" + String(LoRa_param_1.spreadingFactor),
              "bw:" + String(signalBandwidth_values[LoRa_param_1.signalBandwidth_id]/1000.0) + "kHz");
              break;
      case 3 : if ( WiFi.status() == WL_CONNECTED )
                  oled_print(OC16, "WiFi " + WiFi.SSID(), (String)WiFi.localIP()[0] + "." + (String)WiFi.localIP()[1] + "." +
                  (String)WiFi.localIP()[2] + "." + (String)WiFi.localIP()[3], WiFi.macAddress(), "RSSI " + (String)WiFi.RSSI() + " dBm");
               else
                 oled_print(OC24, NO_WIFI1, NO_WIFI2, "", ""); 
              break;
      default: if ( display_nb-4 < ENR_NB ) {
                  oled_print(OC16,enr[display_nb-4].mess, enr[display_nb-4].object, enr[display_nb-4].rssi, enr[display_nb-4].snr);
               } else {
                  display_nb = 0; 
                  display.displayOff();
               }
              break;
    }
  }


}
// ***********************************************************************
// ** end loop
// ***********************************************************************
