#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

///--- TSL PINS ASSIGNEMENT

//--	TSL2591 wiring
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vin to 3.3V
// connect GROUND to GND

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

///--- FUNCTIONS
void launchTslProcess();

// Initiate mod with pins management and communication speed
void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println();
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
  
  ///--- LAUNCHING TSL SENSOR PROCESS
  launchTslProcess();

  delay(500);
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
