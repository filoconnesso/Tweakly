/*
*
*  DIGITAL TOGGLE OUTPUT PAD FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 13

//Set pin 11 for led on PWM_OUTPUT mode
Pad led(LED_PIN);

void setup() {
  //Invert current pin value
  led.toggle();
  delay(2000);
  //Invert current pin value
  led.toggle();
  delay(2000);
  //Invert current pin value
  led.toggle();
  delay(2000);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}