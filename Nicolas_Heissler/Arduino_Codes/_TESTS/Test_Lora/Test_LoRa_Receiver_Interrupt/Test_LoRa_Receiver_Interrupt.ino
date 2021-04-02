#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

///--- CONST
#define LoraReveicerBand 866E6
#define measure 13
#define UP   1
#define DOWN 0

// Lora Receiver
#define receiver_rst  14
#define receiver_dio0 27
#define receiver_ss   4   // 5 ESP, 18 TTGO
#define SCK           18    // 18 ESP, 5 TTGO
#define MISO          19
#define MOSI          27   // 23 ESP, 27 TTGO
#define CS            4   // 5 ESP,  18 TTGO

///--- VARIABLES
int messageNumber = 0;
esp_sleep_source_t wakeUpReason;        //reason of the waking up
RTC_DATA_ATTR int alreadyUp = 0;

///--- FUNCTIONS
void launchLoraReceiverProcess();

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  pinMode (CS,      OUTPUT);
  pinMode (measure, OUTPUT);
  pinMode(receiver_dio0, INPUT);
  
  digitalWrite(measure, HIGH);

  SPI.begin();
  //if(! alreadyUp)
  //{
    Serial.println("LoRa Receiver");
    LoRa.setPins(receiver_ss, receiver_rst, receiver_dio0);
    LoRa.setSPIFrequency(8E6);
    digitalWrite(CS, LOW);
    
    while (!LoRa.begin(LoraReveicerBand)) 
    {
      Serial.println("Starting LoRa failed !");
      delay(1000);
    }
    LoRa.receive();
    Serial.println("Starting LoRa sucessfully !");
    //alreadyUp = 1;
  //}


  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, UP);
}

void loop()
{
//  LoRa.onTxDone(launchLoraReceiverProcess);
  wakeUpReason = esp_sleep_get_wakeup_cause();
  switch(wakeUpReason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 :
      Serial.printf("Waked up by external interruption, launching Lora Receiver process\n");
      delay(1000);
      launchLoraReceiverProcess();
    break;
    
    default :
      Serial.printf("Wake up was not caused by deepsleep : %d\n", wakeUpReason);
    break;
  }
  Serial.println("=== goto sleep ======");
  
  esp_deep_sleep_start();
  
}

void launchLoraReceiverProcess()
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
