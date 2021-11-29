/*
*
*  DIGITAL INPUT PAD FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define BUTTON_PIN 12

//Set pin 12 for button on INPUT_PULLUP mode
Pad button(BUTTON_PIN, INPUT_PULLUP);

void setup() {
  //Start serial
  Serial.begin(115200);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Print input value, this is bounced method
  Serial.println(button.read());
  //Put your code here :-)
}