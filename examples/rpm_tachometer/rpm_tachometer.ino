/*
*
*  TACHOMETER EXAMPLE FOR ARDUINO TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define HALL_SENSOR 3

int turns = 0;
int rpm = 0;

void setup() {
  //Start serial
  Serial.begin(9600);
  //Init pin for hall sensor
  padMode(HALL_SENSOR,INPUT_PULLUP ,LOW);
  //Init interrupt on the hall sensor pin
  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR), addTurn, CHANGE);
  //Start tick for rpm calculation
  setTick("calculateRPM", 1000, calculateRPM);
}

void addTurn() {
  //Add new turn
  turns++;
}

void calculateRPM() {
  //Disable interrupt
  noInterrupts();
  //Calculate rpm
  rpm = turns * 60;
  //Set turns to 0
  turns = 0;
  //Enable interrupt
  interrupts();
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Print the rpm on the serial
  Serial.print("RPM : ");
  Serial.println(rpm);
  //Put your code here :-)
}