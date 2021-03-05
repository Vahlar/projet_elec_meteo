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

// Lora Receiver
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

  //Initiate sender LoRa device
  Serial.println("LoRa sender");
  digitalWrite(measure, HIGH);
  LoRa0.setPins(sender_ss, sender_rst, sender_dio0);
  while (!LoRa0.begin(LoraSenderBand)) 
  {
    Serial.println("Starting sender LoRa failed !");
    delay(500);
  }
  Serial.println("Starting sender LoRa successfully !");
//  digitalWrite(measure, LOW);
  delay(1000);

/*
  //Initiate receiver LoRa device
  Serial.println("LoRa1 sender");
  LoRa1.setPins(sender2_ss, sender2_rst, sender2_dio0);  
  while (!LoRa1.begin(LoraSenderBand)) 
  {
    Serial.println("Starting sender 2 LoRa failed !");
    delay(500);
  }
  Serial.println("Starting sender 2 LoRa successfully !");
  */
}

void loop()
{  
  Serial.print("Sending packet: ");
  Serial.println(messageNumber);

  //Send packet code
//  digitalWrite(measure, HIGH);
  LoRa0.beginPacket();
  Serial.println("test 1");
  LoRa0.print("Message from first sender number : ");
  LoRa0.print(messageNumber);
  Serial.println("test 2");
  LoRa0.endPacket();
  Serial.println("test 3");
//  digitalWrite(measure, LOW);

  //Sender 2 packet code
//  LoRa1.beginPacket();
//  LoRa1.print("Message from sender 2 number : ");
//  LoRa1.print(messageNumber);
//  LoRa1.endPacket();
 
 
  messageNumber++;
  delay(3000);
}
