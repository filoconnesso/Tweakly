/*
*
*  TICK POLICE BLINK EXAMPLE FOR ARDUINO TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define RED_LED 12
#define BLUE_LED 11

void setup() {
  //Set pin 12 as output and turn off it
  padMode(RED_LED,OUTPUT,LOW);
  //Set pin 11 as output and turn off it
  padMode(BLUE_LED,OUTPUT,LOW);
  //Start tick timer on the red function 
  setTick("red",50,red);
  //Start tick timer on the blue function 
  setTick("blue",100,blue);
}

void red() {
  //I switch pin 12 status
  digitalToggle(RED_LED);
}

void blue() {
  //I switch pin 11 status
  digitalToggle(BLUE_LED);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put your code here :-)
}