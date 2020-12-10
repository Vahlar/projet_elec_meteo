#include <Wire.h> 
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
# define PinG 32 // ->32-39

///--- VARIABLES 
long measurementPeriod = 1000;           //sampling period in ms
unsigned long instant = 0;               //current time
unsigned long previous_loop = 0;         //time of the last loop
float Direction = 0, tension = 0;       //wind direction

///--- FUNCTIONS

// Initiate mod with pins management and communication speed

void setup()
{
  
  pinMode(PinG, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();

}

void loop()
{
  instant = millis();
  
  if ((instant - previous_loop) >= measurementPeriod)     //if the measurement period has passed
  {
    //analogSetAttenuation(ADC_11db);                                           
    //tension = analogRead(PinG);
    //Serial.println(tension); 

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11);
    tension = adc1_get_raw(ADC1_CHANNEL_0);
    Serial.println(tension); 
                                                              
    //Display of the direction
    //Serial.print("wind direction: ");   
    //Serial.println(Direction);                                  
    
    previous_loop = instant;     //resets the time and counter for the next measurement period
   }
  
}
