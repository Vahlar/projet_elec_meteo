#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

///--- CONST
#define LoraReveicerBand 868E6

// Lora Receiver
//#define reveicer_rst  
//#define reveicer_dio0 
//#define reveicer_ss   5

///--- VARIABLES
int messageNumber = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

 // LoRa.setPins(reveicer_ss, reveicer_rst, reveicer_dio0);
  while (!LoRa.begin(LoraReveicerBand)) 
  {
    Serial.println("Starting LoRa failed !");
  }
  
  Serial.println("Starting LoRa sucessfully !");
}

void loop()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  // received a packet
  if (packetSize) 
  {
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available())
    {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
