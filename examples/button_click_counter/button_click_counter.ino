/*
*
*  CLICK COUNTER EXAMPLE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define BTN_PIN 12

int clicks = 0;
bool pressed = false;

void setup() {
  //Start serial
  Serial.begin(9600);
  //Init pin for button
  padMode(BTN_PIN,INPUT_PULLUP,LOW);
}

void addClick() {
  clicks++;
  Serial.println(clicks);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Get button status, this method is bouncing free
  if(digitalPushButton(BTN_PIN)) {
    if(!pressed) {
      addClick();
      pressed = true;
    }
  } else {
    pressed = false;
  }
  //Put your code here :-)
}
