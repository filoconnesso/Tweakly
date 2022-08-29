/*
 * 
 * ADJUST TIMER TO PAD EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 * Functionality to straighten the timing of the pads, 
 * if you need to customize and optimize the response times of events on the pads, you can use the padSettings object.
 * 
 */
 #include "Tweakly.h"

 //Create button Pad
 Pad button(8, INPUT);

 padSettings myButtonSettings;

 void setup() {
  
  //Start serial
  Serial.begin(115200);
  
  //Set the debouncing time to 25 milliseconds.
  myButtonSettings.debounceTimer = 25;
  
  //Set the double click event timeout to 400 millisecons.
  myButtonSettings.doubleClickTimer = 400;

  //Set the long press time to 2000 milliseconds.
  //Tweakly automatically corrects the relationship between the double click timer from the moment 
  //it recognizes that a long double click is being performed. (long click timer - double click timeout)
  myButtonSettings.longPressTimer = 2000;
  
  //Applies the settings contained in myButtonSettings to the button.
  button.adjust(myButtonSettings);

  //Attach click event to button Pad
  button.onEvent(CLICK, [] {
    Serial.println("click!");
  });

  //Attach double click event to button Pad
  button.onEvent(DOUBLE_CLICK, [] {
    Serial.println("double click!");
  });

  //Attach long press event to button Pad
  button.onEvent(LONG_PRESS, [] {
    Serial.println("long press!");
  });
  
 }
 
 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }