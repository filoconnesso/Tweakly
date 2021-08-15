/*
 * 
 * TICKTIMER EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create clock
 Clock myclock;

 void setup() {
  //Start serial
  Serial.begin(115200);
  //Start timers
  
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }
