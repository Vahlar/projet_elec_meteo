#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

///--- CONST
#define LoraReveicerBand 866E6

// Lora Receiver
#define receiver_rst  14
#define receiver_dio0 26
#define receiver_ss   5 // 5 ESP, 18 TTGO
#define SCK           18  // 18 ESP, 5 TTGO
#define MISO          19
#define MOSI          23  // 23 ESP, 27 TTGO
#define CS            5   // 5 ESP,  18 TTGO

///--- VARIABLES
int messageNumber = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  SPI.begin(SCK,MISO,MOSI,CS);

  LoRa.setPins(receiver_ss, receiver_rst, receiver_dio0);
  
  while (!LoRa.begin(LoraReveicerBand)) 
  {
    Serial.println("Starting LoRa failed !");
    delay(1000);
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
    Serial.println("");
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
