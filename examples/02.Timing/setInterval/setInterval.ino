/*
 * 
 * TICKTIMER INTERVAL VARIATION EXAMPLE FOR TWEAKLY
 * Created By Mirko Pacioni
 * 
 */
 #include "Tweakly.h"

 //Create timers
 TickTimer timer;
 doList intervalTasks;

 void setup() {
  //Start serial
  Serial.begin(115200);
  //I create some tasks with various setInterval
  intervalTasks.addTask([] { timer.setInterval(100); });
  intervalTasks.addTask([] { timer.setInterval(300); });
  intervalTasks.addTask([] { timer.setInterval(600); });
  intervalTasks.addTask([] { timer.setInterval(900); });
  //Start timer
  timer.attach(900, []{ 
    Serial.println("This message will be show with a different interval"); 
    intervalTasks.next();
  });
 }

 void loop() {
  //Call Tweakly for ever
  TweaklyRun();
  //Put your code here :-)
 }