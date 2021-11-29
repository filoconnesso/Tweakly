/*
 * 
 * DOLIST TASKS EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create list
 doList myTasks;

 //Create timer
 TickTimer timer;

 int counterVal = 0;

 void setup() {
  //Start Serial
  Serial.begin(115200);
  
  //Add tasks on list
  myTasks.addTask([] { counterVal++; });
  myTasks.addTask([] { Serial.println(counterVal); });

  //Start timer
  timer.attach(500, []{ myTasks.next(); });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }