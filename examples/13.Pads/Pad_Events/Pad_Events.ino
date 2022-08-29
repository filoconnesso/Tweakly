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

 void setup() {
  
  //Start serial
  Serial.begin(115200);

  //Attach click event to button Pad
  button.onEvent(CLICK, [] {
    Serial.println("click!");
    led.toggle();
  });

  //Attach double click event to button Pad
  button.onEvent(DOUBLE_CLICK, [] {
    Serial.println("double click!");
  });

  //Attach long press event to button Pad
  button.onEvent(LONG_PRESS, [] {
    Serial.println("long press!");
  });

  //Attach release event to button Pad
  button.onEvent(RELEASE, [] {
    Serial.println("release!");
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
 
 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }