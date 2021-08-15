/*
 * 
 * DIGITALTOGGLECLASS EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad red(12, OUTPUT, "led");
 Pad white(11, OUTPUT, "led");
 Pad green(10, OUTPUT, "led");

 //Create timer
 TickTimer blinker;

 void setup() {
  //Start timer
  blinker.attach(500, []{ digitalToggleClass("led"); });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }