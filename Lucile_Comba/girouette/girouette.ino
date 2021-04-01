#include <Wire.h> 
#include <SPI.h>
#include <driver/adc.h>
//#include "esp_adc_cal.h"

///--- Définitions des PINS
# define PinG 34// ->32-39

///--- VARIABLES 
long measurementPeriod = 1000;           //sampling period in ms
unsigned long instant = 0;               //current time
unsigned long previous_loop = 0;         //time of the last loop
float tension = 0, tension_alim = 0;       //tensions
float coef_ajustement = 0;
long sum;

///--- FUNCTIONS

// Initiate mod with pins management and communication speed

void setup()
{
  
  //pinMode(PinG, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();

}

void loop()
{
  instant = millis();
  
  if ((instant - previous_loop) >= measurementPeriod)     //if the measurement period has passed
  {
    //adc_power_on();
    //adc1_config_width(ADC_WIDTH_12Bit);
    //adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);
    
    sum = 0;
    for (int i = 0; i<20; i++) {
      //tension = adc1_get_raw(ADC1_CHANNEL_6);
      tension = analogRead(PinG);
      sum += tension;
    }
    
    tension = sum / 20;
    
    void analogReference(DEFAULT);   //tension d'alimentation réel
    coef_ajustement = 3.3/tension_alim;     //coef de variation par rapport à la tension attendu d'alimentation 3,3
   
    Serial.println(float((tension * 3.9)/4095));
   
    
    /*tension = adc1_get_raw(ADC1_CHANNEL_6);
    Serial.println(tension);
    tension = (tension * 3.9)/4095;
    Serial.println(tension); 
    
    tension = analogRead(PinG); Serial.print("analog : ");
    Serial.println((tension* 3.9)/4095); 
    */                                                    
    //Display of the direction
    //Serial.print("wind direction: ");   
    //Serial.println(Direction);                                  
    
    previous_loop = instant;     //resets the time and counter for the next measurement period
   }
  
}
