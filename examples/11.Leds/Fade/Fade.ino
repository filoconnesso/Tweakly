/*
*
*  NO BLOCKING FADE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 6

//Set pin 6 for led
Pad led(LED_PIN, PWM_OUTPUT);

void setup() {
  //Setup code
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //analogWriteProgressive function for fade effect
  analogWriteProgressive(LED_PIN, 1000, PULSE_TO_EDGE);
  //Put your code here :-)
}