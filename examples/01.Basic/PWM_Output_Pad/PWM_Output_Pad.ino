/*
*
*  PWM OUTPUT PAD FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 11

//Set pin 11 for led on PWM_OUTPUT mode
Pad led(LED_PIN, PWM_OUTPUT);

void setup() {
  //Write 0 on pin 11
  led.write(0);
  delay(2000);
  //Write 20 on pin 11
  led.write(20);
  delay(2000);
  //Write 40 on pin 11
  led.write(40);
  delay(2000);
  //Write 60 on pin 11
  led.write(60);
  delay(2000);
  //Write 80 on pin 11
  led.write(80);
  delay(2000);
  //Write 100 on pin 11
  led.write(100);
  delay(2000);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}