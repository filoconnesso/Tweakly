/*
*
*  BUTTON MULTI STATUS EXAMPLE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define BTN_PIN 12

void setup() {
  //Start serial
  Serial.begin(9600);
  //Init pin for button
  padMode(BTN_PIN,INPUT_PULLUP,LOW);
  //Start tick to print button status
  setTick("printbtnstatus", 300, printBtnStatus);
}

void printBtnStatus() {
  //Print all button status on the serial, both methods are bouncing free
  if(digitalPushButton(BTN_PIN)) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button released");
  }
  if(digitalSwitchButton(BTN_PIN)) {
    Serial.println("Switch ON");
  } else {
    Serial.println("Switch OFF");
  }
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}
