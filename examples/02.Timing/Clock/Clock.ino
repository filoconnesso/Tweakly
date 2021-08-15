/*
 * 
 * CLOCK EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create clock
 Clock myclock;

 //Create Echo object
 Echo printer;

 //Create Timer
 TickTimer printerTimer;

 void setup() {
  //Start serial
  Serial.begin(115200);
  //Begin Echo
  printer.attach(&Serial);
  //Start clock with computer time
  myclock.autoSync();
  /*------OR------
  myclock.setHours(18);
  myclock.setMinutes(0);
  myclock.setSeconds(0);
  */
  printerTimer.attach(1500, printClock);
 }

 void printClock() {
     printer.print("%d:%d:%d\n",myclock.getHours(), myclock.getMinutes(), myclock.getSeconds());
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }