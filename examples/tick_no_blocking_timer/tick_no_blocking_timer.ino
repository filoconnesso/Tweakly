/*
*
*  TICK TIMER EXAMPLE FOR ARDUINO TWEAKLY
*  Created By Mirko Pacioni
*
*/

#include "Tweakly.h"

#define BTN_PIN 12

int counter = 0;

void setup()
{
  //Start serial
  Serial.begin(9600);
  //Set the restart timer button
  padMode(BTN_PIN, INPUT_PULLUP, LOW);
  //Start ticks timer
  setTick("random", 200, randomNumbers);
  //Start random seed
  randomSeed(analogRead(0));
}

void randomNumbers()
{
  //Print random number on the serial and add counter
  Serial.println(random(100, 200));
  counter++;
}

void loop()
{
  //Call Tweakly forever
  TweaklyRun();
  //Pause the timer when the counter reaches 10
  if (counter >= 10)
  {
    pauseTick("random");
  }
  //Press the button for restart tick timer, the button will be inhibited until the tick is running
  if (!tickIsRunning("random"))
  {
    if (digitalPushButton(BTN_PIN))
    {
      playTick("random");
      counter = 0;
    }
  }
  //Put your code here :-)
}