#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

///--- CONST
#define LoraSenderBand 866E6
#define measure 13

LoRaClass LoRa0;
LoRaClass LoRa1;

// Lora Sender
#define sender_rst    14
#define sender_dio0   26
#define sender_ss     5
#define sender_cs     5

// Lora Sender2
#define sender2_rst  14
#define sender2_dio0 27
#define sender2_ss   4
#define sender2_cs   4

// SCK, MISO, MOSI is the same pin for both LoRa devices
#define SCK           18 
#define MISO          19
#define MOSI          27

///--- VARIABLES
int messageNumber = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  //Define both chip select as output
  pinMode (sender_cs,   OUTPUT);
  pinMode (sender2_cs,  OUTPUT);
  pinMode (measure,     OUTPUT);

  SPI.begin();
  
  digitalWrite(sender_cs, HIGH);
  digitalWrite(sender2_cs, HIGH);

  //Initiate sender LoRa device
  Serial.println("LoRa sender");
  digitalWrite(measure, HIGH);
  digitalWrite(sender_cs, LOW);
  digitalWrite(sender2_cs, HIGH);
  delay(1000);
  LoRa0.setPins(sender_ss, sender_rst, sender_dio0);
  LoRa0.setSPIFrequency(8E6);
  while (!LoRa0.begin(LoraSenderBand)) 
  {
    Serial.println("Starting sender LoRa failed !");
    delay(1000);
  }
  Serial.println("Starting sender LoRa successfully !");
  digitalWrite(sender_cs, HIGH);
  delay(1000);
  
  //Initiate sender 2 LoRa device
  Serial.println("LoRa1 sender");
  digitalWrite(sender2_cs, LOW);
  LoRa1.setPins(sender2_ss, sender2_rst, sender2_dio0);  
  LoRa1.setSPIFrequency(8E6);
  while (!LoRa1.begin(LoraSenderBand)) 
  {
    Serial.println("Starting sender 2 LoRa failed !");
    delay(500);
  }
  Serial.println("Starting sender 2 LoRa successfully !");
  digitalWrite(sender2_cs, HIGH);
  delay(1000);
}

void loop()
{
  //Sender 1 packet code
  Serial.print("Sending packet from sender 1 : ");
  Serial.println(messageNumber);
  
  //digitalWrite(sender_cs, LOW);
  LoRa0.begin(LoraSenderBand);
  delay(500);
  LoRa0.beginPacket();
  LoRa0.print("Message from first sender number : ");
  LoRa0.print(messageNumber);
  LoRa0.endPacket(true);
  //digitalWrite(sender_cs, HIGH);
  delay(1000);
  
  //Sender 2 packet code
  Serial.print("Sending packet from sender 2 : ");
  Serial.println(messageNumber);
  
  //digitalWrite(sender2_cs, LOW);
  LoRa1.begin(LoraSenderBand);
  delay(500);
  LoRa1.beginPacket();
  digitalWrite(sender_cs, HIGH);
  digitalWrite(sender2_cs, LOW);
  LoRa1.print("Message from sender 2 number : ");
  LoRa1.print(messageNumber);
  LoRa1.endPacket(true);
  //digitalWrite(sender2_cs, HIGH);
 
  messageNumber++;
  delay(3000);
}
