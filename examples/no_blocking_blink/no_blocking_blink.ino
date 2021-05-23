/*
*
*  NO BLOCKING BLINK FOR ARDUINO TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 13

void setup() {
  //Set pin 13 as output and turn off it
  padMode(LED_PIN,OUTPUT,LOW);
  //Start tick timer on the blink function 
  setTick("blink", 1000, blink);
}

void blink() {
  //I switch pin 13 status
  digitalToggle(LED_PIN);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}