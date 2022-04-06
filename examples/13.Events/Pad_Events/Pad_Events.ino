/*
 * 
 * PAD EVENTS EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 * Tweakly monitors events on pins, 
 * this is a modern feature that allows you to perform an operation when a pin's state changes.
 * 
 */
 #include "Tweakly.h"

 //Create button Pad
 Pad button(12, INPUT_PULLUP);
 
 //Create led Pad
 Pad led(13);

 //Create blinker timer
 TickTimer blinker;

 void setup() {
  
  //Start serial
  Serial.begin(115200);

  //Start blinker timer
  blinker.attach(2000, blink);

  //Attach click event to button Pad
  button.onEvent(CLICK, [] {
    Serial.println("click");
  });

  //Attach release event to button Pad
  button.onEvent(RELEASE, [] {
    Serial.println("release");
  });

  //Attach To On Event to led Pad
  led.onEvent(TO_ON, [] {
    Serial.println("led on!");
  });

  //Attach To Off Event to led Pad
  led.onEvent(TO_OFF, [] {
    Serial.println("led off!");
  });
  
 }

 //blink is the callback function for timer
 void blink() {
  //Toggle led
  led.toggle();
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }