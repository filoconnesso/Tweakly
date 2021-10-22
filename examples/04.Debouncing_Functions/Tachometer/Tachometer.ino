/*
 * TACHOMETER EXAMPLE FOR TWEAKLY
 * Created by Mirko Pacioni
 * 
 */

//Include Tweakly library
#include <Tweakly.h>

//Echo object
Echo printer;
//Pins
Pad sensor(8, INPUT_PULLUP);
Pad led(13);
//Timer
TickTimer timer;

int r, rm, rs = 0;
bool increment = true;

void setup() {
  Serial.begin(115200);
  printer.attach(&Serial);
  //Attach timer with 1 second timeout
  timer.attach(1000, tachometer);
}

void tachometer() {
  //Rotations for second
  rs = r;
  //Rotations for minute
  rm = r * 60;
  //Reset rotations counter
  r = 0;
}

void loop() {
  //Call TweaklyRun for ever
  TweaklyRun();

  //Get digital sensor input
  int input = digitalPushButton(sensor.pinNumber());

  //If input is true
  if(input) {
    if(increment) {
      //Increase rotation counter
      r++;
      //Turn on led
      led = 1;
      //Disable rotation counter increment
      increment = false;
    }
  } else {
    //If input is false
    //Turn off led
    led = 0;
    //Enable rotation counter increment for next input
    increment = true;
  }

  //Print values
  printer.print("R/m : ~%d | R/s : %d \n", rm, rs);

  //Put your code here :-)
}
