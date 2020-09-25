#include <config.h>
#include <LoRaNode.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_BME280.h>

#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include "cactus_io_BME280_I2C.h"
#include "Adafruit_TSL2591.h"
#include "images.h"

//Définitions des PINS
#define SCK     18    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    23   // GPIO27 -- SX1278's MOSI
#define SS      5   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     2   // GPIO26 -- SX1278's IRQ(Interrupt Request)
//Définition de la bande de fréquence LORA sur la badande européene 868 Mhz
#define BAND  868E6

/*    TLS2591 WIRING    */
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vin to 3.3V
// connect GROUND to GND

/*    BME280 WIRING    */
// connect SCL to I2C Clock (PIN 36 on ESP32)
// connect SDA to I2C Data  (PIN 33 on ESP32)
// connect Vcc to 3.3V
// connect GROUND to GND

BME280_I2C bme(0x76); // I2C using address 0x76
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

int t, t0 = 0, deltaT = 2000;
unsigned long Balise_DernierEnvoi = 0; // heure du dernier envoi LoRa
unsigned int counter = 0;

void setup() //initialisation des modules avec initialisation des PINS, de la vitesse de communication en liaison série
{
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(866E6)) //test pour savoir si le module a réussi à démarrer
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("init ok");
  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  bme.setTempCal(-1);// Temp was reading high so subtract 1 degree
  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  if (tsl.begin())
  {
    Serial.println(F("Found a TSL2591 sensor"));
  }
  else
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
  tsl.setGain(TSL2591_GAIN_MED);  //Medium gain (LOW for bright light and HIGH for dim light)
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); //Medium intergration time  (100MS for bright light to 600MS for dim light)
}

void loop()
{
  String Message = "";
  t = millis();
  if (t - t0 > deltaT)
  {
    t0 = t;
    bme.readSensor();

    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;

    Serial.print("\n");
    Serial.print("Pression en millibars : ");
    Serial.println(bme.getPressure_MB());

    Serial.print("Humidité en % : ");
    Serial.println(bme.getHumidity());

    Serial.print("Temperature en degre celsius : ");
    Serial.println(bme.getTemperature_C());

    Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir));

    //Lora
    if (Serial.available())
    {
      while (Serial.available())
      {
        Message += Serial.readString();
      }
      //Serial.println(Message);
      counter = 0;
      LoRa.beginPacket();
      Serial.print("Moi: ");
      Serial.println(Message);
      LoRa.print(Message);
      //LoRa.print(counter);
      LoRa.endPacket();
    }

    int packetSize = LoRa.parsePacket();   // onregarde si quelque chose est reçu
  }
}
