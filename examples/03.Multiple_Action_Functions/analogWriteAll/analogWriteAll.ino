/*
 * 
 * ANALOGWRITEALL EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad led1(11, PWM_OUTPUT);
 Pad led2(10, PWM_OUTPUT);
 Pad led3(9, PWM_OUTPUT);

 void setup() {
  //PWM writing
  analogWriteAll(20);
  delay(400);
  analogWriteAll(40);
  delay(400);
  analogWriteAll(60);
  delay(400);
  analogWriteAll(80);
  delay(400);
  analogWriteAll(100);
  delay(400);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }