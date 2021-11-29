/*
 * 
 * ENCODER BUTTON EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad clk(7, INPUT_PULLUP);
 Pad dt(8, INPUT_PULLUP);
 Pad button(12, INPUT_PULLUP);
 
 void setup() {
  //Start Serial
  Serial.begin(115200);
  //Set encoder
  encoderAttach(dt.pinNumber(), clk.pinNumber(), onEncoderChange);
 }

 void onEncoderChange(bool dir) {
  //if dir is true
  if(dir) {
    //Print left on serial
    Serial.println("LEFT");
  } else {
    //Print right on serial
    Serial.println("RIGHT");
  }
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Get button status
  bool btn = digitalPushButton(button.pinNumber());
  if(btn) {
    Serial.println("PRESSED");
  }
  //Put your code here :-)
 }