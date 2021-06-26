/*
 * 
 * ANALOGWRITECLASS EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad led1(11, PWM_OUTPUT, "led");
 Pad led2(10, PWM_OUTPUT, "led");
 Pad led3(9, PWM_OUTPUT, "led");

 void setup() {
  //PWM writing
  analogWriteClass("led", 20);
  delay(400);
  analogWriteClass("led", 40);
  delay(400);
  analogWriteClass("led", 60);
  delay(400);
  analogWriteClass("led", 80);
  delay(400);
  analogWriteClass("led", 100);
  delay(400);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }