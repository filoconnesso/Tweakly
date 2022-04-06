/*
 * 
 * TWO SERVOS EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * With this example it is possible to synchronously control two or more servos
 * 
 */
#include <Tweakly.h>
#include <Servo.h>

//Create two servos object
Servo servo1;
Servo servo2;

//Create a TickTimer
TickTimer servoTimer;

//Create the position variable
int pos = 0;

//Create the variable that allows you to reverse the position change
bool add = true;

void setup() {
  
  //Set the servo pins
  servo1.attach(8);
  servo2.attach(7);
  
  //Set the servos to position 0
  servo1.write(0);
  servo2.write(0);
  
  //Start the servo timer with an interval of 10ms
  servoTimer.attach(10, [] {
    //If pos is equal to 180 it sets the add variable to false
    if(pos == 180) {
      add = false;
    }
    //If pos is equal to 0 it sets the add variable to true
    if(pos == 0) {
      add = true;
    }
    //If the add variable is true it increases the value of pos, 
    //if the add variable is false it decreases the value of pos
    if(add) {
      pos++;
    } else {
      pos--;
    }
    //Set servos position with pos value
    servo1.write(pos);
    servo2.write(pos);
  });

  //The timer will take care of changing the position of the two servos while you can do other things in the loop

}

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }