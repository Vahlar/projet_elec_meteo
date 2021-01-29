#include <Wire.h>
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
#define PinP 34
#define UP   1
#define DOWN 0

///--- Variables
time_t now;
char strftime_buf[64];

RTC_DATA_ATTR int nbRocker = 0;         //number of rockers (saved on RTC memory)
RTC_DATA_ATTR time_t lastTimeDataSent;
RTC_DATA_ATTR unsigned long timePassed; 
float qtyRain = 0;                      //quantity of rain (mm)
esp_sleep_source_t wakeUpReason;        //reason of the waking up

void setup() 
{
  pinMode(PinP, INPUT_PULLDOWN);
  Serial.begin(9600);
  Serial.println("");
  while (!Serial);

  //Enable waking up with external interruption
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, DOWN);
}

void loop() 
{
  time(&now);
  
  wakeUpReason = esp_sleep_get_wakeup_cause();
  switch(wakeUpReason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 :
      nbRocker += 1;
      Serial.printf("Waked up by external interruption : %d \n",nbRocker);
      Serial.printf("Current time : %d s \n", now);
      timePassed = difftime(now, lastTimeDataSent) * 1000000; //timePassed need to be in us
      Serial.printf("Data passed since last send : %l us \n",timePassed);
      esp_sleep_enable_timer_wakeup(10000000ULL - timePassed);
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
      time(&lastTimeDataSent);
      esp_sleep_enable_timer_wakeup(10000000ULL - timePassed);
    break;

    default :
      Serial.printf("Wake up was not caused by deepsleep : %d\n", wakeUpReason);
      esp_sleep_enable_timer_wakeup(10000000ULL - timePassed);      
    break;
  }
  Serial.println("=== goto sleep ======");

  esp_deep_sleep_start();

}
