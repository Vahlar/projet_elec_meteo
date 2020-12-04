#include <Wire.h> 
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
# define PinA 35

///--- VARIABLES 
int nbInterrupt = 0;   //number of interruption & interruption per second
long measurementPeriod = 1000;           //sampling period in ms
unsigned long instant = 0;               //current time
unsigned long previous_loop = 0;         //time of the last loop
unsigned long previous_interrupt = 0;    //time of the last interruption
float Speed = 0, interruptSec = 0;       //wind speed

///--- FUNCTIONS
void launchAnemometerProcess();


// Initiate mod with pins management and communication speed
void setup()
{
  
  
  pinMode(PinA, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println();

  //at each lap: call of the function to increment the number of rotation
  attachInterrupt(digitalPinToInterrupt(PinA), launchAnemometerProcess, FALLING);

}

void loop()
{
  instant = millis();
  //Serial.println(nbRotation);
  
  if ((instant - previous_loop) >= measurementPeriod)     //if the measurement period has passed
  {
    interruptSec = nbInterrupt / (measurementPeriod * 0.001);   //A wind speed of 2.4 km/h causes the switch to close once per second
    Speed = interruptSec*2.4;                                  // measurementPeriod * 0.001 is used to define interrupt/sec depending on the periode              
                                                                
    //Display of the speed
    //Serial.println(nbInterrupt);
    //Serial.println(interruptSec);
    Serial.print("wind speed: ");   
    Serial.print(Speed);                                  
    Serial.println(" km/h");
    
    previous_loop = instant;     //resets the time and counter for the next measurement period
    nbInterrupt = 0;
   }
  
}

void launchAnemometerProcess()                                                             
{
  if(previous_interrupt == 0 || instant-previous_interrupt >= 5) // if it is the first interruption or the next one after 5 ms (debouncing)
  {
    previous_interrupt = instant;
    nbInterrupt = nbInterrupt + 1;  //increment of the number of interruptions
  }
}
