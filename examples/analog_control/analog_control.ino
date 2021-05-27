/*

   ANALOG CONTROL FOR TWEAKLY
   Created By Mirko Pacioni

*/

#include "Tweakly.h"

#define ANALOG_LED_PIN_ONE 11
#define ANALOG_LED_PIN_TWO 10

#define PWM_LOW 20
#define PWM_MIDDLE 120
#define PWM_HIGH 255

int curExample = 0;

void setup() {
  //Set analog pins
  analogPadMode(ANALOG_LED_PIN_ONE, 0, 0, 255, "led_class_a");
  analogPadMode(ANALOG_LED_PIN_TWO, 0, 0, 255, "led_class_b");
  //Start tick timers
  setTick("change_example", 1000, change_example);
}

void change_example() {
  //Change example number
  if (curExample != 6) {
    curExample++;
  } else {
    curExample = 0;
  }
}

void loop() {
  //Call Tweakly forever
  TweaklyRun();
  //PWM examples
  switch (curExample) {
    case 1 :
      analogWriteAll(PWM_LOW);
      break;
    case 2 :
      analogWriteAll(PWM_MIDDLE);
      break;
    case 3 :
      analogWriteAll(PWM_HIGH);
      break;
    case 4 :
      analogWriteClass("led_class_a", PWM_HIGH);
      analogWriteClass("led_class_b", PWM_LOW);
      break;
    case 5 :
      analogWriteClass("led_class_a", PWM_MIDDLE);
      analogWriteClass("led_class_b", PWM_MIDDLE);
      break;
    case 6 :
      analogWriteClass("led_class_a", PWM_LOW);
      analogWriteClass("led_class_b", PWM_HIGH);
      break;
  }
  //Put your code here :-)
}
