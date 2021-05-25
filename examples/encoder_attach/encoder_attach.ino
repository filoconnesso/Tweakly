/*
*
*  ENCODER EXAMPLE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define DT_PIN 2
#define CLK_PIN 3
#define BTN_PIN 4
#define LED_PIN 13

int counter = 0;

void setup() {
  //Start serial
  Serial.begin(9600);
  //Set pin
  padMode(LED_PIN,OUTPUT,LOW);
  padMode(DT_PIN,INPUT,LOW);
  padMode(CLK_PIN,INPUT,LOW);
  padMode(BTN_PIN,INPUT,LOW);
  //Connect the encoder to the change function 
  encoderAttach(DT_PIN, CLK_PIN, change);
  //Start blink tick timer
  setTick("blink", 100, blink);
}

void blink() {
  //Toggle led pin
  digitalToggle(LED_PIN);
}

void change(bool dir) {
  /*
  * The callback of the encoders will generate a boolean reference value, 
  * if the value is 1 the encoder is turning from one direction, 
  * if the value is 0 the encoder is turning from the opposite direction. 
  * The variable in question can be used to determine the true, 
  * increase or decrease a numerical value and much more.
  * 
  * The callback function is only called when the encoder is rotating. 
  */
  if(!dir) {
    Serial.println("left");
    counter++;
  } else {
    Serial.println("right");
    counter--;
  }
  //Print the counter
  Serial.println(counter);
}

void loop() {
  //Call Tweakly forever!
  TweaklyRun();
  
  //If the switch connected to the encoder is up TRUE, the timer tick connected to the blink stops. 
  if(digitalSwitchButton(BTN_PIN)) {
    pauseTick("blink");
  } else {
    playTick("blink");
  }
  //Put your code here :-)
}
