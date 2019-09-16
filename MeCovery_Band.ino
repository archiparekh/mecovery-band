#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include <Adafruit_CircuitPlayground.h>
//#include "BluefruitConfig.h"

#define SLOUCH_ANGLE        10.0      // allowable slouch angle (deg)
#define GRAVITY             9.80665   // standard gravity (m/s^s)
#define RAD2DEG             52.29578  // convert radians to degrees

float low, high, currentAngle, greatestAngle;
bool doExercise, setTargetAngle;
int numberOfMistakes, numberOfGoods; 
void setup() {
  CircuitPlayground.begin();
  doExercise = false;
  setTargetAngle = false;
  numberOfMistakes = 0; 
  numberOfGoods = 0; 
  CircuitPlayground.setBrightness(25);
}

void loop() {

  if ( (CircuitPlayground.rightButton()) )
  {
    //Serial.println ("Start Measuring Angle!");

    if ( setTargetAngle == false )
    {
      //Serial.println ("If Loop of Right Button!");
      
      setTargetAngle = true;
      CircuitPlayground.playTone(900,100);

      greatestAngle = 0;
      
      delay(500);
    }
    else
    {
      //Serial.println ("Else Loop of Right Button!");
      setTargetAngle = false;
      for(int i = 0; i <10; i++){                           //1 green flash, signals end of target angle setting
            CircuitPlayground.setPixelColor(i, 0, 50, 0);
          }
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
    }

     //Serial.println(greatestAngle);
  }

  if ( setTargetAngle == true )
  {
    currentAngle = RAD2DEG * asin(-CircuitPlayground.motionZ() / GRAVITY);
    if (currentAngle < greatestAngle)
      greatestAngle = currentAngle;
  }
  
  if ((CircuitPlayground.leftButton()) )
  {
      //Serial.println(CircuitPlayground.leftButton());
      
      if( doExercise == false )
      {
          //Serial.println("Start Exercise!");
          doExercise = true;
          CircuitPlayground.playTone(900,100);
          low = greatestAngle + 10; 
          high = greatestAngle - 10;

          delay(500);
      }
      else
      {
         //Serial.println("End Exercise!");
         // Serial.println(numberOfGoods - numberOfMistakes);
         /* for(int j = 0; j < 3; j++){
            for(int i = 0; i <10; i++){
              CircuitPlayground.setPixelColor(i, 0, 50, 0);
            }
            delay(1000);
          }
          */
         CircuitPlayground.clearPixels(); //Clears red arrow from doExercise
       
         for (int j = 0; j < 3; j++){
          for (int i = 0; i < 10; i++){
            CircuitPlayground.setPixelColor(i, 0, 100, 0);
          }
          delay(200);
          CircuitPlayground.clearPixels();      
         }
          
         doExercise = false;
         low = 0;
         high = 0;
         currentAngle = 0;
        
         delay(500);
      }
  }

  if (doExercise == true )
  {
    currentAngle = RAD2DEG * asin(-CircuitPlayground.motionZ() / GRAVITY);
    
    CircuitPlayground.clearPixels();      //Clears pixels prior to each testing of the angle
    
    if (currentAngle < high )
    {
      //Serial.println (currentAngle);
     // CircuitPlayground.playTone(900,100); 
     // CircuitPlayground.clearPixels();
     for (int i = 0; i < 10; i++){
            CircuitPlayground.setPixelColor(i, 0, 0, 0);
          }
      for(int i = 3; i < 7; i++)              //red arrow
        CircuitPlayground.setPixelColor(i, 200, 0, 0); 
      numberOfMistakes++;  
    }
    else{
      for (int i = 0; i < 10; i++)
        CircuitPlayground.setPixelColor(i, 0, 50, 0);
      numberOfGoods++;
    } 
  }
}
