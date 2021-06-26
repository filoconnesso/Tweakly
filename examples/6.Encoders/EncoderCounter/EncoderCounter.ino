/*
 * 
 * ENCODER COUNTER EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad clk(7, INPUT_PULLUP);
 Pad dt(8, INPUT_PULLUP);

 int counterVal = 0;
 
 void setup() {
  //Start Serial
  Serial.begin(9600);
  //Set encoder
  encoderAttach(dt.pinNumber(), clk.pinNumber(), onEncoderChange);
 }

 void onEncoderChange(bool dir) {
  //if dir is true
  if(dir) {
    //Add value to counterVal
    counterVal++;
  } else {
    //Else remove value to counterVal
    counterVal--;
  }
  //Print value of counterVal
  Serial.println(counterVal);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }