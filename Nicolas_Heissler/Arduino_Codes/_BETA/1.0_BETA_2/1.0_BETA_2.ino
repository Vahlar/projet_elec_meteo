///--- LIBRARY INCLUDE
#include <Wire.h>
#include <SPI.h>
#include <config.h>
#include <LoRa.h>
#include <driver/adc.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_BME280.h>

///--- PINS ASSIGNEMENT
#define PinP 32 //GPIO 32

//---  RFM95W (LORA SENDER) 
#define sender_rst    14
#define sender_dio0   26
#define sender_ss     5
#define sender_cs     5

//--- RFM95W (LORA RECEIVER)
#define receiver_rst  14
#define receiver_dio0 27
#define receiver_ss   4
#define receiver_cs   4

// SCK, MISO, MOSI is the same pin for both LoRa devices
#define SCK     18  // GPIO5  -- SX1278's SCK
#define MISO    19  // GPIO19 -- SX1278's MISnO
#define MOSI    27  // GPIO27 -- SX1278's MOSI

//---  TSL2591 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vin to 3.3V
// connect GROUND to GND

//---  BME280 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect VCC to 3.3V
// connect GROUND to GND

///--- CONSTANT
#define UP   1
#define DOWN 0
#define measure 13
#define SEALEVELPRESSURE_HPA (1013.25)
#define SENDERBAND  868E6 // Frequency to send (868 Mhz)
#define RECEIVERBAND  868E6 // Frequency to send (868 Mhz)

///--- VARIABLE
time_t now;
char strftime_buf[64];

Adafruit_BME280 bme;  // I2C
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
LoRaClass LoRa0;
LoRaClass LoRa1;

unsigned long actualTime;                   // Actual time (in ms since the programme was launched
unsigned long TimeOfTheLastSend = 0;        // Time where the last message was sent
int           TimeBetweenTwoSend = 900000;  // Time between two sending (900000 ms -> 15 minutes)
unsigned long counter = 0;                  // Number of message sent
String        messageReceived = "";         // Message that the balance module will send to us
String        messageToSend = "";           // Message to send to the gateway

RTC_DATA_ATTR int nbRocker = 0;         //number of rockers (saved on RTC memory)
RTC_DATA_ATTR time_t lastTimeDataSent;
RTC_DATA_ATTR unsigned long timePassed; 
float qtyRain = 0;                      //quantity of rain (mm)

esp_sleep_source_t wakeUpReason;        //reason of the waking up

///--- FUNCTIONS
void launchBmeProcess();
void launchTslProcess();
void launchRainProcess();

void setup() 
{
  //-- SETTING SERIAL CONNECTION
  Serial.begin(9600);
  Serial.println("");
  while (!Serial);
  Serial.println("Beginning all module settings...");

  //-- SETTING INTERRUPT PINS
  pinMode (PinP, INPUT_PULLDOWN);
  pinMode (sender_cs,   OUTPUT);
  pinMode (receiver_cs,  OUTPUT);
  pinMode (measure,     OUTPUT);

  SPI.begin();
  digitalWrite(measure, HIGH);
    
  //-- SETTING BME280 SENSOR
  while (!bme.begin(0x77, &Wire))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  Serial.println("BME280 is founded and set");
  
  //-- SETTING TSL2591 SENSOR
  while (!tsl.begin())
  {
  Serial.println(F("No sensor found ... check your wiring?"));
  }
  Serial.println(F("TSL2591 sensor founded and set"));
  //-- CONFIGURING THE TSL2591   
  tsl.setGain(TSL2591_GAIN_MED);  //Medium gain (LOW for bright light and HIGH for dim light)
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); //Medium intergration time  (100MS for bright light to 600MS for dim light)

  //-- SETTING LORA SENDER MODULE
  Serial.println("LoRa sender");
  digitalWrite(sender_cs, LOW);
  digitalWrite(receiver_cs, HIGH);
  delay(1000);
  LoRa0.setPins(sender_ss, sender_rst, sender_dio0);
  LoRa0.setSPIFrequency(8E6);
  while (!LoRa0.begin(SENDERBAND)) 
  {
    Serial.println("Starting sender LoRa failed !");
    delay(1000);
  }
  Serial.println("Starting sender LoRa successfully !");
  digitalWrite(sender_cs, HIGH);
  delay(1000);

  //-- SETTING LORA RECEIVER MODULE
  Serial.println("LoRa1 receiver");
  digitalWrite(receiver_cs, LOW);
  LoRa1.setPins(receiver_ss, receiver_rst, receiver_dio0);  
  LoRa1.setSPIFrequency(8E6);
  while (!LoRa1.begin(RECEIVERBAND)) 
  {
    Serial.println("Starting receiver LoRa failed !");
    delay(500);
  }
  Serial.println("Starting receiver LoRa successfully !");
  digitalWrite(receiver_cs, HIGH);
  delay(1000);

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
      Serial.println("Waked up by timer, display datas then send them to the gateway :");

      //-- LAUNCH ALL SENSOR PROCESS
      launchBmeProcess();
      launchTslProcess();
      launchRainProcess();
      
      //-- SEND THE DATA
      launchLoRaProcess();
      
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

void launchBmeProcess()
{
  bme.takeForcedMeasurement();
  
  //-- DISPLAY PRESSURE
  Serial.print("Pressure : ");
  Serial.print(bme.readPressure());
  Serial.println(" hPa");

  //-- DISPLAY HUMIDITY
  Serial.print("Humidity : ");
  Serial.println(bme.readHumidity());
  Serial.println(" %");
    
  //-- DISPLAY TEMPERATURE
  Serial.print("Temperature : ");
  Serial.println(bme.readTemperature());
  Serial.println(" °C");

  //-- DISPLAY ALTITUDE
  Serial.print("Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
}

void launchTslProcess()
{
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir), 6);
}

void launchRainProcess()
{
  //-- Calculate the quantity of rain on the period
  qtyRain = 0.2794*nbRocker;
  
  //-- Display of the number of rockers & the rain quantity
  Serial.printf("Number of rockers : %d\n", nbRocker);
  Serial.printf("Quantity of rain : %f L/m²\n", qtyRain);
  nbRocker = 0;
}

void launchLoRaProcess()
{
  //-- SENDING DATA TO THE GATEWAY
  Serial.print("Sending packet to the gateway : ");
  
  LoRa0.begin(LoraSenderBand);
  delay(500);
  LoRa0.beginPacket();
  LoRa0.print("W");
  LoRa0.print("MAC::ADDRESSE");
  //-- EACH DATA NEED TO BE SEND ON ONE BYTE
  //-- TWO BYTE FOR THE TEMP
  LoRa0.endPacket(true);
  delay(1000);
}
