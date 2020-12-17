/*---------------------------------------------------------------------------------------------
-     Les interruptions causées par le pluviometre reset le timer de 10 s
-     -> Tant que des interruptions sont détectées il n'y aura pas d'envoi d'infos
-     
-
-     Chercher un moyen de savoir le temps entre l'interruption et le début du timer
-     TEST 1 (sauvegarder le timer dans l'eeprom)
---------------------------------------------------------------------------------------------*/

#include <Wire.h>
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
#define PinP 34
#define UP   1
#define DOWN 0

///--- VARIABLES 
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) before sending informations */

RTC_DATA_ATTR int nbRocker = 0;         //number of rockers (saved on RTC memory)
RTC_DATA_ATTR unsigned long lastTimeDataSent = 0;
RTC_DATA_ATTR unsigned long timePassed = 0; 
float qtyRain = 0;                      //quantity of rain (mm)
esp_sleep_source_t wakeUpReason;        //reason of the waking up

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer();

void setup()
{
  pinMode(PinP, INPUT_PULLDOWN);
  Serial.begin(9600);
  Serial.println("");
  while (!Serial);

  //Enable waking up with a timer
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR - timePassed);  
  esp_err_t pdConfigError = esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  if (pdConfigError != ESP_OK) 
  {
    Serial.println("pdConfigError");
  }
  //Enable waking up with external interruption
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, DOWN);
}

void loop()
{
  wakeUpReason = esp_sleep_get_wakeup_cause();
  switch(wakeUpReason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 :
      Serial.print("Waked up by external interruption : ");
      nbRocker += 1;
      Serial.println(nbRocker);
      timePassed = system_get_time() - lastTimeDataSent;
    break;

    case ESP_SLEEP_WAKEUP_TIMER :
      Serial.println("Waked up by timer, display datas :");
      // Calculate the quantity of rain on the period
      qtyRain = 0.2794*nbRocker;
      // Display of the number of rockers & the rain quantity
      Serial.printf("Number of rockers : %d\n", nbRocker);
      Serial.printf("Quantity of rain : %f L/m²\n", qtyRain);
      nbRocker = 0;
      timePassed = 0;
      lastTimeDataSent = system_get_time();
      esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR - timePassed);  
    break;

    default :
      Serial.printf("Wake up was not caused by deepsleep : %d\n", wakeUpReason);      
    break;
  }
  esp_deep_sleep_start();
  

}
