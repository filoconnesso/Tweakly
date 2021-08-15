/*
 * 
 * CLICK COUNTER EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad led(13);
 Pad button(12, INPUT_PULLUP);

 int clickCounter = 0;
 bool pressed = false;

 void setup() {
  Serial.begin(115200);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  if(digitalPushButton(button.pinNumber())) {
    if(!pressed) {
      clickCounter++;
      Serial.println(clickCounter);
      pressed = true;
    }
  } else {
    pressed = false;
  }
  //Put your code here :-)
 }