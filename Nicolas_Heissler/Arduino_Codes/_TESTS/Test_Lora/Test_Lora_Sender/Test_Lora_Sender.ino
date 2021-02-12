#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

///--- CONST
#define LoraSenderBand 866E6

// Lora Sender
#define sender_rst  14
#define sender_dio0 26
#define sender_ss   18 // 5 ESP, 18 TTGO
#define SCK         5  // 18 ESP, 5 TTGO
#define MISO        19
#define MOSI        27  // 23 ESP, 27 TTGO
#define CS          18

///--- VARIABLES
int messageNumber = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  SPI.begin(SCK,MISO,MOSI,CS);
  
  LoRa.setPins(sender_ss, sender_rst, sender_dio0);
  
  while (!LoRa.begin(LoraSenderBand)) 
  {
    Serial.println("Starting LoRa failed !");
    delay(500);
  }
  
  Serial.println("Starting LoRa sucessfully !");
}

void loop()
{
  Serial.print("Sending packet: ");
  Serial.println(messageNumber);

  // send packet
  LoRa.beginPacket();
  LoRa.print("Aloha ");
  LoRa.print(messageNumber);
  LoRa.endPacket();

  messageNumber++;

  delay(5000);
}
