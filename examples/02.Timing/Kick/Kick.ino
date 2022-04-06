/*
 * 
 * KICK TICKTIMER EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create timers
 TickTimer timer;
 Pad led(13);
 Pad button(12, INPUT_PULLUP);

 void setup() {
  //Start serial
  Serial.begin(115200);
  //Start the timer and disable it 
  timer.attach(2000, turnOff, DISPATCH_OFF);
 } 

 void turnOff() {
   //Turn off the LED when it is called up
   led.off();
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();

  //When the button is pressed, the led lights up, 
  //the attention of the timer is requested with kick.
  //Kick differs from dispatchNow() in that it does not immediately execute the timer callback. 
  //With kick, the activation time of the TickTimer is respected
  if(digitalPushButton(button.pinNumber())) {
    led.on();
    timer.kick();
  }

  //After the kick, the timer is enabled and its counter reset, 
  //in this case the LED will turn off 2 seconds after the last button was pressed
  
  //Put your code here :-)
  
 }