#include <Wire.h> 
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
# define PinA 35

///--- VARIABLES 
int nbRotation = 0;            //number of rotation
long measurementPeriod = 1500;           //sampling period in ms
unsigned long instant = 0;              //current time
unsigned long previous_millis = 0;        //stores the instant variable
float Speed = 0;                        //wind speed

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
  attachInterrupt(digitalPinToInterrupt(PinA), launchAnemometerProcess, RISING);

}

void loop()
{
  instant = millis();
  //Serial.println(nbRotation);
  
  if ((instant - previous_millis) >= measurementPeriod)     //if the measurement period has passed
  {
    Speed = (nbRotation / (measurementPeriod * 0.001)) * 2.4;   //A wind speed of 2.4 km/h causes the switch to close once per second
                                                                // measurementPeriod * 0.001 is used to define interrupt/sec depending on the periode
    //Display of the speed
    Serial.println(nbRotation);
    Serial.print("wind speed: ");   
    Serial.print(Speed);                                  
    Serial.println(" km/h");
    
    previous_millis = instant;     //resets the time and counter for the next measurement period
    nbRotation = 0;
   }
  
}

void launchAnemometerProcess()                                                             
{
  nbRotation = nbRotation + 1;                //increment of the number of rotations
}
