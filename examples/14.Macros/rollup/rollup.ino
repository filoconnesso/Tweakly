/*
 * 
 * ROLLUP EXAMPLE (BLINK) FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 * The rollup macro allows you to turn off a portion of code within the loop for a certain period of time. 
 * Tweakly Rollup is a non-blocking alternative to classic delay.
 * to rewind the desired portion of code, the time to be controlled must be indicated with the sleep_for(milliseconds) macro.
 * 
 */
 #include "Tweakly.h"

 //Create led Pad
 Pad led(6);

 void setup() {
  
  //Start serial
  Serial.begin(115200);
  
 }
 
 void loop() {
  //Call Tweakly for ever
  TweaklyRun();

  //Start rollup
  //The first rollup interrupts part of the code for 1000 milliseconds, 
  //when the time is exceeded the current state of the LED is switched.
TWEAKLY_ROLLUP
  sleep_for(1000);
  led.toggle();
END_ROLLUP
  //Finish rollup

  //Start rollup
  //The second rollup prints the current status of the LED every 3000 milliseconds.
TWEAKLY_ROLLUP
  sleep_for(3000);
  Serial.print("led status = ");
  Serial.print(led.read());
  Serial.print("\n");
END_ROLLUP
  //Finish rollup
  
  //Your code continues without interruption
  //Put your code here :-)
 }