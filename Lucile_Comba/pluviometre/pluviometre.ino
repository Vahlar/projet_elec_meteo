#include <Wire.h> 
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
# define PinP 34

///--- VARIABLES 
unsigned long nbRocker = 0;           //number of rockers
long measurementPeriod = 1500;            //sampling period in ms
unsigned long instant = 0;            //current time
unsigned long previous_millis = 0;            //stores the instant variable            

///--- FUNCTIONS
void launchRainGaugeProcess();


// Initiate mod with pins management and communication speed
void setup()
{
  pinMode(PinP, INPUT_PULLDOWN);
  Serial.begin(9600);
  while (!Serial);

  //at each tilting: call of the function to increment the number of rockers
  attachInterrupt(digitalPinToInterrupt(PinP), launchRainGaugeProcess, CHANGE);

}

void loop()
{
  instant = millis();
  if ((instant - previous_millis) >= measurementPeriod)     //if the measurement period has passed
  {
    // Display of the number of rockers
    Serial.print("Number of rockers : ");
    Serial.println(nbRocker);
    
    previous_millis = instant;    //resets the time and counter for the next measurement period
    nbRocker = 0;
   }
  
}

void launchRainGaugeProcess()                  
{                                      
  nbRocker += 1;          //increment of the number of rockers
}
