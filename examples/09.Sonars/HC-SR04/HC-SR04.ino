
/*
*
*  HC-SR04 SONAR EXAMPLE FOR TWEAKLY
*  Created By Mirko Pacioni
*
*/
#include "Tweakly.h"

//Set sonar pin
Pad echoPin(7, INPUT);
Pad triggerPin(8, OUTPUT);

//Create Echo object
Echo serialPrinter;

void setup() {
  
  //Start serial
  Serial.begin(115200);
  
  //Attach serial to Echo object
  serialPrinter.attach(&Serial);

  //Attach sonar
  sonarAttach(echoPin.pinNumber(), triggerPin.pinNumber(), getSonarValue, SONAR_CENTIMETERS);

}

void getSonarValue(unsigned long centimeters) {
  //Print sonar value
  serialPrinter.print("%ld cm \n", centimeters);
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put Your Code Here :-)
}
