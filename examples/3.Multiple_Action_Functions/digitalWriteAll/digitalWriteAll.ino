/*
 * 
 * DIGITALWRITEALL EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad red(12);
 Pad white(11);
 Pad green(10);

 void setup() {
  //Start blink all leds
  digitalWriteAll(HIGH);
  delay(200);
  digitalWriteAll(LOW);
  delay(200);
  digitalWriteAll(HIGH);
  delay(200);
  digitalWriteAll(LOW);
  delay(200);
  digitalWriteAll(HIGH);
  delay(200);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }