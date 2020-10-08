#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TSL2591.h>

///--- Définitions des PINS

//-- BME PINS ASSIGNEMENTS

//--  BME280 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vcc to 3.3V
// connect GROUND to GND

//--  TSL2591 wiring
// connect SCL to I2C Clock (PIN 14 on ESP32)
// connect SDA to I2C Data  (PIN 13 on ESP32)
// connect Vin to 3.3V
// connect GROUND to GND

///--- CONST
#define SEALEVELPRESSURE_HPA (1022)

///--- FUNCTIONS
void launchTslProcess();
void launchBmeProcess();

Adafruit_BME280 bme;  // I2C
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

// Initiate mod with pins management and communication speed
void setup()
{
  //pinMode(DI0, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();

  ///--- BME
  Serial.println("Beginning BME280 module settings...");

  // Try to find BME280 sensor
  while (!bme.begin(0x77, &Wire))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  Serial.println("BME280 is founded and set");

  ///--- TSL
  Serial.println("Beginning TSL2591 module settings...");

  // Try to find Adafruit TSL2591
  while (!tsl.begin())
  {
  Serial.println(F("No sensor found ... check your wiring?"));
  }
  // Configures the gain and integration time for the TSL2591   
  tsl.setGain(TSL2591_GAIN_MED);  //Medium gain (LOW for bright light and HIGH for dim light)
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); //Medium intergration time  (100MS for bright light to 600MS for dim light)
  Serial.println(F("TSL2591 sensor founded and set"));
}

void loop()
{
  Serial.print("\n");

  //-- LAUNCHING BME SENSOR PROCESS
  launchBmeProcess();
  //--- LAUNCHING TSL SENSOR PROCESS
  launchTslProcess();
  
  delay(2000);
}

///--- FUNCTIONS
void launchBmeProcess()
{
  bme.takeForcedMeasurement();
  
  //-- DISPLAY PRESSURE
  Serial.print("Pressure : ");
  Serial.print(bme.readPressure());
  Serial.println(" Pa");
  Serial.print("Pression au niveau 0 : ");
  Serial.print(bme.readPressure()/0.987);
  Serial.println(" Pa");

  //-- DISPLAY HUMIDITY
  Serial.print("Humidity : ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
    
  //-- DISPLAY TEMPERATURE
  Serial.print("Temperature : ");
  Serial.print(bme.readTemperature());
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
