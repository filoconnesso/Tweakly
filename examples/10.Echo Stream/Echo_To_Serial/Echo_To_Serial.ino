/*
*
*  ECHO TO SERIAL EXAMPLE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/
#include "Tweakly.h"

//Create Echo object
Echo printer;

void setup() {
  
  //Start serial
  Serial.begin(115200);
  
  //Attach serial to Echo object
  printer.attach(&Serial);

}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Print to Serial
  printer.print("%s \n", "Hello, Friend!");
  //Put Your Code Here :-)
}
