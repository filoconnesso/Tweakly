/*
 * 
 * FIRE GUN EFFECT EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Define pin
 #define BUTTON_PIN 2
 #define BUZZER_PIN 4
 
 //Create pads
 Pad button(BUTTON_PIN, INPUT_PULLUP);
 Pad buzzer(BUZZER_PIN, MELODY_OUTPUT);

 //Create player
 Player myPlayer;
 
 void setup() {
   //Attach buzzer pin
   myPlayer.buzzerAttach(BUZZER_PIN);
   //Attach default firegun effect
   myPlayer.melodyAttach(firegun_song);
   //Set delay and duration for notes
   myPlayer.setNoteDelay(32);
   myPlayer.setNoteDuration(15);
   //Set repeat to false
   myPlayer.repeat(false);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Get button status
  bool btn = digitalPushButton(BUTTON_PIN);
  if(btn) {
    //Play sound effect
    myPlayer.play();
  }
  //Put your code here :-)
 }