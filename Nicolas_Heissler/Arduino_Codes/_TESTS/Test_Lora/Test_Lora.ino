#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

///--- CONST
#define LoraSenderBand 868E6

///--- VARIABLES
int messageNumber = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  while (!LoRa.begin(LoraSenderBand)) 
  {
    Serial.println("Starting LoRa failed !");
  }
  
  Serial.println("Starting LoRa sucessfully !");
}

void loop()
{
  Serial.print("Sending packet: ");
  Serial.println(messageNumber);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(messageNumber);
  LoRa.endPacket();

  messageNumber++;

  delay(5000);
}
