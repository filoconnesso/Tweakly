/*
*
*  DIGITAL WRITE CLASS EXAMPLE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_ONE 13
#define LED_TWO 12
#define LED_THREE 11
#define LED_FOUR 10

#define A_CLASS "A"
#define B_CLASS "B"

void setup()
{
  //Start serial
  Serial.begin(9600);

  //Init pad for four leds
  padMode(LED_ONE, OUTPUT, LOW, A_CLASS);
  padMode(LED_TWO, OUTPUT, LOW, A_CLASS);
  padMode(LED_THREE, OUTPUT, HIGH, B_CLASS);
  padMode(LED_FOUR, OUTPUT, HIGH, B_CLASS);

  //Set HIGH for all output pins on A class
  digitalWriteClass(A_CLASS,HIGH);
  //Set LOW for all output pins on B class
  digitalWriteClass(B_CLASS,LOW);

}

void loop()
{
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}
