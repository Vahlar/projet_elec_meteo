#include <Wire.h> 
#include <SPI.h>
#include <driver/adc.h>

///--- Définitions des PINS
# define PinP 34

///--- VARIABLES 
unsigned long nbRocker = 0;             //number of rockers
float qtyRain = 0;             //quantity of rain (mm)
long measurementPeriod = 5000;          //sampling period in ms
unsigned long instant = 0;              //current time
unsigned long previous_loop = 0;        //time of the last loop
unsigned long previous_interrupt = 0;   //time of the last interruption            

///--- FUNCTIONS
void launchRainGaugeProcess();


// Initiate mod with pins management and communication speed
void setup()
{
  pinMode(PinP, INPUT_PULLDOWN);
  Serial.begin(9600);
  Serial.println("");
  while (!Serial);

  //at each tilting: call of the function to increment the number of rockers
  attachInterrupt(digitalPinToInterrupt(PinP), launchRainGaugeProcess, FALLING);

}

void loop()
{
  instant = millis();
  if ((instant - previous_loop) >= measurementPeriod)     //if the measurement period has passed
  {
    // Calculate the quantity of rain on the period
    qtyRain = 0.2794*nbRocker;
    // Display of the number of rockers & the rain quantity
    Serial.print("Number of rockers : ");
    Serial.println(nbRocker);
    Serial.print("Quantity of rain : ");
    Serial.print(qtyRain);    
    Serial.println(" L/m²");
    
    previous_loop = instant;    //resets the time and counter for the next measurement period
    nbRocker = 0;
   }
  
}

void launchRainGaugeProcess()                  
{                                      
  if(previous_interrupt == 0 || instant-previous_interrupt >= 5) // if it is the first interruption or the next one after 5 ms (debouncing)
  {
    previous_interrupt = instant;
    nbRocker += 1;          //increment of the number of rockers
  }
}
