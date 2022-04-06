/*

   WATCHTIME EXAMPLE FOR TWEAKLY
   Created By Mirko Pacioni

*/
#include "Tweakly.h"

//Create timers
TickTimer timer;

bool CRITIC = true;

void setup() {
  //Start serial
  Serial.begin(115200);
  //Start timer
  timer.attach(1000, [] {
    //Print this messager every 1 second
    Serial.println("Hi, I'am timer!");
    Serial.println(millis());
  });
}

void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Enter in critic zone
  if (CRITIC) {
    //Start WatchTime
    timer.startWatch();
    //Delay 2 seconds
    delay(2000);
    //Stop WatchTime
    timer.stopWatch();
    //Print WatchTime
    Serial.print("Done in : ");
    Serial.print(timer.getWatchTime());
    Serial.print("ms \n");
    //Disable critic zone
    CRITIC = false;
  }
  
  //Put your code here :-)
}