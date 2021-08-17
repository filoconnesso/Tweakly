/*
 * 
 * TICKTIMER EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create timers
 TickTimer timer;

 void setup() {
  //Start serial
  Serial.begin(115200);
  //Start timers
  timer.attach(500, [] { Serial.println("Hello, Friend!"); });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }
