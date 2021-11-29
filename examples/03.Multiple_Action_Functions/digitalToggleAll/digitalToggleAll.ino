/*
 * 
 * DIGITALTOGGLEALL EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad red(12);
 Pad white(11);
 Pad green(10);

 //Create timer
 TickTimer blinker;

 void setup() {
  //Start timer
  blinker.attach(500, []{ digitalToggleAll(); });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }