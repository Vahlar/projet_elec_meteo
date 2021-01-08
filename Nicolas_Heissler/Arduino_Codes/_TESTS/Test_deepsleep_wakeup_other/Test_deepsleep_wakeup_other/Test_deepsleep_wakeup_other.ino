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
struct tm timeinfo;
int hours,minutes,seconds;

RTC_DATA_ATTR int nbRocker = 0;         //number of rockers (saved on RTC memory)
RTC_DATA_ATTR unsigned long lastTimeDataSent;
RTC_DATA_ATTR unsigned long timePassed; 
float qtyRain = 0;                      //quantity of rain (mm)
esp_sleep_source_t wakeUpReason;        //reason of the waking up

unsigned long timePassed(char cValue[]);

void setup() 
{
  // put your setup code here, to run once:
  // Set timezone to China Standard Time
  setenv("TZ", "CST-8", 1);
  tzset();

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
      Serial.print("Waked up by external interruption : ");
      nbRocker += 1;
      Serial.println(nbRocker);
      localtime_r(&now, &timeinfo);
      strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
      Serial.println(strftime_buf);
      timePassed = timePassed(strftime_buf);
      Serial.println(timePassed);
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
      localtime_r(&lastTimeDataSent, &timeinfo);
      esp_sleep_enable_timer_wakeup(10000000ULL - timePassed);
    break;

    default :
      Serial.printf("Wake up was not caused by deepsleep : %d\n", wakeUpReason);
      esp_sleep_enable_timer_wakeup(10000000ULL - timePassed);      
    break;
  }

/*localtime_r(&now, &timeinfo);
strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
//ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);  
Serial.println("=== wakeup ======");
Serial.println(strftime_buf);
delay(1100);
  esp_sleep_enable_timer_wakeup(10000000ULL);
  */
  Serial.println("=== goto sleep ======");

  esp_deep_sleep_start();

}

unsigned long timePassed(char cValue[])
{
 // Hours are 12 & 13
 hours = atoi(cValue[12]&cValue[13]);
 // Minutes are 15 & 16
 minutes = atoi(cValue[15]&cValue[16]);
 // Seconds are 18 & 19
 seconds = atoi(cValue[18]&cValue[19]);
  Serial.print(hours);Serial.print(minutes);Serial.print(seconds);

  return 0;
}
