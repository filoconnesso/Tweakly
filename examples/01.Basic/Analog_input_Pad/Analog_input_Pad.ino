/*
*
*  ANALOG INPUT PAD FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define PHOTORESISTOR_PIN 0

//Set pin 0 (A0) for photoresistor on ANALOG_INPUT mode
Pad photoresistor(PHOTORESISTOR_PIN, ANALOG_INPUT);

void setup() {
  //Start serial
  Serial.begin(115200);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Print photoresistor value on serial
  Serial.println(photoresistor.read());
  //Put your code here :-)
}