/*
 * 
 * INPUT HUNTER DEMO EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create pads
 Pad button1(12, INPUT_PULLUP, "buttons");
 Pad button2(11, INPUT_PULLUP, "buttons");
 Pad button3(10, INPUT_PULLUP, "buttons");

 //Create inputHunter
 inputHunter hunty;
 
 void setup() {
  //Start Serial
  Serial.begin(115200);
  //Set inputHunter
  hunty.assign("buttons", onHunterChange);
 }

 void onHunterChange(int pin) {
   //Print pin number after class change
   Serial.println(pin);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Input Hunter Recognition
  hunty.recognition();
  //Put your code here :-)
 }