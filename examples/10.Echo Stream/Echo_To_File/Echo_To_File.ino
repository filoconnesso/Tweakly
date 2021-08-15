/*

   ECHO TO FILE EXAMPLE FOR TWEAKLY
   Created By Mirko Pacioni

*/
#include "Tweakly.h"

//Include SD libs
#include <SPI.h>
#include <SD.h>

//Create file object
File file;

//Create Echo object
Echo printer;

//Create timer
TickTimer printerTimer;

void setup() {

  //Start serial
  Serial.begin(115200);

  //Begin SD Card
  if (!SD.begin(4)) {
    Serial.println("SD Card Failed!");
    return;
  } else {
    Serial.println("SD Card OK!");
  }

  //Attach timer
  printerTimer.attach(1000, filePrinter);

}

void filePrinter() {
  //Begin file
  file = SD.open("tweakly.txt", FILE_WRITE);
  if (file) {
    //Attach file to Echo object
    printer.attach(&file);
    //Print on file
    printer.print("%s \n", "Hello, Friend!");
    //Close file
    file.close();
  }
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //Put Your Code Here :-)
}