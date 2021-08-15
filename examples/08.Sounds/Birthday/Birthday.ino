/*
 * 
 * BIRTHDAY MELODY EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Define pin
 #define PLAY_BUTTON_PIN 2
 #define STOP_BUTTON_PIN 3
 #define BUZZER_PIN 4

 //Birthday melody
int birthday[] = {
  NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,
  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2, 0, 0, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,
  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_END
};
 
 //Create pads
 Pad play_btn(PLAY_BUTTON_PIN, INPUT_PULLUP);
 Pad stop_btn(STOP_BUTTON_PIN, INPUT_PULLUP);
 Pad buzzer(BUZZER_PIN, MELODY_OUTPUT);

 //Create player
 Player myPlayer;
 
 void setup() {
   //Attach buzzer pin
   myPlayer.buzzerAttach(BUZZER_PIN);
   //Attach default birthday melody
   myPlayer.melodyAttach(birthday);
   //Set delay and duration for notes
   myPlayer.setNoteDelay(200);
   myPlayer.setNoteDuration(50);
   //Set repeat to false
   myPlayer.repeat(false);
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Get play button status
  bool playbtn = digitalPushButton(PLAY_BUTTON_PIN);
  if(playbtn) {
    //Play melody
    myPlayer.play();
  }
  //Get stop button status
  bool stopbtn = digitalPushButton(STOP_BUTTON_PIN);
  if(stopbtn) {
    //Stop melody
    myPlayer.stop();
  }
  //Put your code here :-)
 }