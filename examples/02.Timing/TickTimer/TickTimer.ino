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
  //Start timer
  timer.attach(1000, []{ 
    Serial.println("Hi, I'am timer!"); 
    Serial.println(millis());
  });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }