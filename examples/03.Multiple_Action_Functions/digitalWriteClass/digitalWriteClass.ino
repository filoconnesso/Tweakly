/*
 * 
 * DIGITALWRITECLASS EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad red(12, OUTPUT, "led");
 Pad white(11, OUTPUT, "led");
 Pad green(10, OUTPUT, "led");

 void setup() {
  //Start blink all leds
  digitalWriteClass("led", HIGH);
  delay(200);
  digitalWriteClass("led", LOW);
  delay(200);
  digitalWriteClass("led", HIGH);
  delay(200);
  digitalWriteClass("led", LOW);
  delay(200);
  digitalWriteClass("led", HIGH);
  delay(200);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }