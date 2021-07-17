/*
 * 
 * TICKTIMER EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create timers
 TickTimer timer1;
 TickTimer timer2;
 TickTimer timer3;

 void setup() {
  //Start serial
  Serial.begin(9600);
  //Start timers
  timer1.attach(200, []{ Serial.println("Hi, I'am timer1!"); });
  timer2.attach(500, []{ Serial.println("Hi, I'am timer2!"); });
  timer3.attach(1500, []{ Serial.println("Hi, I'am timer3!"); });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }
