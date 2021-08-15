/*
 * 
 * TICKTIMER PRIORITY EXAMPLE FOR TWEAKLY
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
  Serial.begin(115200);
  //Start timers
  timer1.attach(200, []{ Serial.println("Hi, I'am timer1!"); });
  timer2.attach(500, []{ Serial.println("Hi, I'am timer2!"); });
  timer3.attach(1500, []{ Serial.println("Hi, I'am timer3!"); });

  //Set the importance of timers
  timer1.setPriority(tweaklyPriorityHighPlus_3);
  timer2.setPriority(tweaklyPriorityMedium_4);
  timer3.setPriority(tweaklyPriorityLow_5);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }
