/*
 * 
 * DISPATCH EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create timers
 TickTimer timerDisabled;
 TickTimer timerOnce;
 TickTimer timerForever;

 void setup() {
  //Start serial
  Serial.begin(115200);
  //Start timer disabled
  timerDisabled.attach(1000, [] { Serial.println("This message will not be printed!"); }, DISPATCH_OFF);
  //Start timer that runs only once
  timerOnce.attach(1000, once, DISPATCH_ONCE);
  //Start timer that runs forever
  timerForever.attach(1000, forever, DISPATCH_FOREVER);
 }

 void once() {
    //Print once
    Serial.println("This text is shown only once");
 }

 void forever() {
    //Print forever
    Serial.println("This text is shown forever");
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //You can instantly invoke the timer and its callback with displatchNow()
  //timerOnce.dispatchNow();
  //or
  //timerDisabled.dispatchNow();

  //Put your code here :-)
 }