/*
*
*  NO BLOCKING LED FADE EFFECT FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 11
#define BLINK_LED 13

void setup() {
  //Set digital pin 13 as output and turn off
  padMode(BLINK_LED, OUTPUT, LOW);
  //Set pwm pin 11 as output and turn off, the minimum and maximum value of the PWM is indicated.
  analogPadMode(LED_PIN,0,0,255);
  //Start tick timer on the blink function 
  setTick("blink", 40, blink);
}

void blink() {
  //Toggle led
  digitalToggle(BLINK_LED);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Enable fade effect to pin
  analogWriteFade(LED_PIN, 2, ALWAYS);
  //Put your code here :-)
}
