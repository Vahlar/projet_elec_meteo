#include <config.h>
//#include <LoRaNode.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_BME280.h>

#include <Wire.h>
//#include <SPI.h>

///--- PINS ASSIGNEMENT

//--	RFM95W (LORA SENDER) 
#define SCK     18  // GPIO5  -- SX1278's SCK
#define MISO    19  // GPIO19 -- SX1278's MISnO
#define MOSI    23  // GPIO27 -- SX1278's MOSI
#define SS      5   // GPIO18 -- SX1278's CS
#define RST     14  // GPIO14 -- SX1278's RESET
#define DI0     2   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  868E6	// Frequency to send (868 Mhz)

//--	TSL2591 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vin to 3.3V
// connect GROUND to GND

//--	BME280 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect VCC to 3.3V
// connect GROUND to GND

///--- VARIABLES
Adafruit_BME280 bme;  // I2C
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

unsigned long actualTime;				 	// Actual time (in ms since the programme was launched
unsigned long TimeOfTheLastSend = 0;	 	// Time where the last message was sent
int 	   	  TimeBetweenTwoSend = 900000;  // Time between two sending (900000 ms -> 15 minutes)
unsigned long counter = 0;				 	// Number of message sent
String		  messageReceived = "";			// Message that the balance module will send to us
String		  messageToSend = "";			// Message to send to the gateway


///--- FUNCTIONS
void launchBmeProcess();

// Initiate mod with pins management and communication speed
void setup() 
{
  pinMode(DI0, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  Serial.println("Beginning all module settings...");
/*
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  
  // Try to init Lora band
  while (!LoRa.begin(BAND)) //test pour savoir si le module a réussi à démarrer
  {
    Serial.println("Starting LoRa on band failed!");
  }
  Serial.println("Lora is now started on band");
*/
  // Try to find BME280 sensor
  while (!bme.begin(0x77, &Wire))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  Serial.println("BME280 is founded and set");
  
  // Try to find Adafruit TSL2591
  while (!tsl.begin())
  {
	Serial.println(F("No sensor found ... check your wiring?"));
  }  
  tsl.setGain(TSL2591_GAIN_MED);  //Medium gain (LOW for bright light and HIGH for dim light)
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); //Medium intergration time  (100MS for bright light to 600MS for dim light)
  Serial.println(F("TSL2591 sensor founded and set"));
}

void loop()
{
  actualTime = millis();
  if (actualTime - TimeOfTheLastSend > TimeBetweenTwoSend)
  {
    TimeOfTheLastSend = actualTime;
	
	//-- LAUNCHING BME SENSOR PROCESS
	launchBmeProcess();

	//-- GET ACTUAL LUMINOSITY
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;

    Serial.print("\n");

	//-- DISPLAY LUMINOSITY
    Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir));
/*
    //-- CHECK IF LORA IS CONNECTED (The receiver)
    if (Serial.available())
    {
	  //-- LORA IS CONNECTED, CHECK IF HE SEND US INFORMATIONS
      while (Serial.available())
      {
        messageReceived += Serial.readString();
      }// WE RECEIVED ALL THE MESSAGE
	  
	  //-- CREATING LORA PACKET TO SEND
	  LoRa.beginPacket();
	  messageToSend = messageReceived;
	  // Print the message for debugging
      Serial.println(messageReceived);
	  // Give the message to the Lora module
      LoRa.print(messageReceived);
      LoRa.endPacket();
	  //-- LORA PACKET CREATED AND SENDED
    }
*/
//    int packetSize = LoRa.parsePacket();   // onregarde si quelque chose est reçu
  }
}

void launchBmeProcess()
{
	bme.takeForcedMeasurement();
	
	//-- DISPLAY PRESSURE
    Serial.print("Pression en millibars : ");
    Serial.println(bme.readPressure());

	//-- DISPLAY HUMIDITY
    Serial.print("Humidité en % : ");
    Serial.println(bme.readHumidity());

	//-- DISPLAY TEMPERATURE
    Serial.print("Temperature en degre celsius : ");
    Serial.println(bme.readTemperature());
}
