/*
*
*  DIGITAL OUTPUT PAD FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 13

//Set pin 13 for led
Pad led(LED_PIN);

void setup() {
  //Turn on pin 13
  led.on(); //or led.write(1);
  delay(2000);
  //Turn off pin 13
  led.off(); //or led.write(0);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}