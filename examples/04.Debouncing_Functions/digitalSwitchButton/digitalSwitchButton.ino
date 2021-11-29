/*
 * 
 * DIGITALPUSHBUTTON EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad led(13);
 Pad button(12, INPUT_PULLUP);

 void setup() {
  //Setup code
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  bool btn = digitalSwitchButton(button.pinNumber());
  led.write(btn);
  //Put your code here :-)
 }