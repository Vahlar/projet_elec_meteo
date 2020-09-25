#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

///--- Définitions des PINS

//-- BME PINS ASSIGNEMENTS

//--	BME280 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vcc to 3.3V
// connect GROUND to GND

///--- VARIABLES
#define SEALEVELPRESSURE_HPA (1013.25)

///--- FUNCTIONS
void launchBmeProcess();

Adafruit_BME280 bme;  // I2C

// Initiate mod with pins management and communication speed
void setup()
{
  //pinMode(DI0, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  Serial.println("Beginning BME280 module settings...");

  // Try to find BME280 sensor
  while (!bme.begin(0x77, &Wire))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  Serial.println("BME280 is founded and set");
}

void loop()
{
  Serial.print("\n");

  //-- LAUNCHING BME SENSOR PROCESS
  launchBmeProcess();
	
  delay(2000);
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
