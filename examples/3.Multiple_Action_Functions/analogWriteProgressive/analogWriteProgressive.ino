/*
 * 
 * ANALOGWRITEPROGRESSIVE EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad led(11, PWM_OUTPUT);

 void setup() {
  //Setup code
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //PWM writing
  analogWriteProgressive(led.pinNumber(), 3000, PULSE_TO_EDGE);
  //Put your code here :-)
 }