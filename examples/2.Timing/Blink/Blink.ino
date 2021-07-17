/*
*
*  NO BLOCKING BLINK FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define LED_PIN 13

//Set pin 13 for led
Pad led(LED_PIN);
//Create blinker TickTimer
TickTimer blinker;

void setup() {
  //Start blinker TickTimer on the blink function 
  blinker.attach(1000, blink);
}

void blink() {
  //I switch pin 13 status
  led.toggle();
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}