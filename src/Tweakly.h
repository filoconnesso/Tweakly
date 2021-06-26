//----------------------------------------------------------------------------//
/*
 *  Copyright (C) 2021  Filo Connesso - filoconnesso.it
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
 //----------------------------------------------------------------------------//

/*
 * LIBRARY FOR WIRING EXSPANSION
 * Created by Mirko Pacioni
 * March 12th 2021
 */

#ifndef TWEAKLY_H
#define TWEAKLY_H

#include <Arduino.h>

// Definitions for analogWriteFade
#define TO_LOW           1                // -> falling effect
#define TO_HIGH          2                // -> rising effect
#define TO_PULSE         3                // -> pulsing effect
#define LOW_TO_EDGE      4                // -> falling effect
#define HIGH_TO_EDGE     5                // -> rising effect
#define PULSE_TO_EDGE    6                // -> pulsing effect

#define PWM_OUTPUT 169
#define ANALOG_INPUT 170

#define LOCK 1
#define UNLOCK 0

typedef void (*_tick_callback)();
typedef void (*_encoder_callback)(bool);
typedef void (*_list_callback)();
typedef void (*_hunter_callback)(int);

volatile bool _tweakly_ready          { false };
volatile bool _ticks_exists           { false };
volatile bool _pad_exists             { false };
volatile bool _encoder_exists         { false };
volatile bool _pwm_pad_exists         { false };
volatile bool _list_exists            { false };
volatile bool _hunter_exists          { false };

// Default values for debaouncing, encoder and pwm
unsigned long _pin_button_default_debounce_millis  = 50;
unsigned long _pin_encoder_default_debounce_millis = 1;
unsigned long _ticks_counter = 0;
unsigned long _lists_counter = 0;

// Struct required for ticks
struct _ticks{
  unsigned long  _tick_current_millis;
  unsigned long  _tick_delay;
  unsigned long  _tick_previous_time;
  unsigned long  _tick_position;
  bool           _tick_enabled;
  _tick_callback _tick_callback_function;
  _ticks *       _next_tick = NULL;
};

// Struct required for pins
struct _pins{
  const char *   _pin_class;
  uint8_t        _pin_number;                                    //pin number 
  uint8_t        _pin_locked;                                                                     
  bool           _pin_status;
  bool           _pin_previous_status;
  bool           _pin_debounced_status;
  bool           _pin_debounced_start_status;
  bool           _pin_switch_status;
  bool           _pin_switch_release_button;
  bool           _pin_old_status;
  uint8_t        _pin_mode;                                      //mode -> INPUT/OUTPUT
  unsigned long  _pin_debounce_current_millis;
  unsigned long  _pin_debounce_previous_millis;
  unsigned long  _pin_debounce_delay_millis;
  _pins *        _next_pin = NULL;
};

// Struct required for pwm pins
struct _pwm_pins{
  const char *   _pwm_pin_class;
  uint8_t        _pwm_pin_number;
  uint8_t        _pwm_pin_locked;
  unsigned int   _pwm_pin_value;
  unsigned long  _pwm_pin_delay_current_millis;
  unsigned long  _pwm_pin_delay_previous_millis;
  bool           _pwm_pin_fade_direction;
  unsigned long  _pwm_max_value;
  unsigned long  _pwm_min_value;
  bool           _pwm_pin_enabled;
  uint8_t        _pwm_fade_mode;
  unsigned long  _pwm_fade_delay;
  bool           _fade_effect;
  _pwm_pins *    _next_pwm_pin = NULL;
};

// Struct required for encoders
struct _encoders{
  const char *   _encoder_name;
  uint8_t        _encoder_dt_pin;
  uint8_t        _encoder_clk_pin;
  bool           _encoder_dt_pin_status;
  bool           _encoder_clk_pin_status;
  bool           _encoder_clk_pin_previous_status;
  unsigned long  _encoder_debounce_current_millis;
  unsigned long  _encoder_debounce_previous_millis;
  unsigned long  _encoder_debounce_delay_millis;
  _encoder_callback _encoder_change_callback;
  _encoders *    _next_encoder = NULL;
};

// Struct required for lists
struct _lists{
  uint16_t       _list_index;
  uint16_t       _functions_counter;
  _list_callback _function_callback;
  _lists *       _next_function = NULL;
};

_ticks    *_first_tick =       NULL,    *_last_tick =  NULL;
_pins     *_first_pin =        NULL,     *_last_pin =  NULL;
_encoders *_first_encoder =    NULL, *_last_encoder =  NULL;
_pwm_pins *_first_pwm_pin =    NULL, *_last_pwm_pin =  NULL;
_lists    *_first_function =   NULL, *_last_function = NULL;

//Class for inputHunters
class inputHunter {
  private :
  uint8_t _this_global_pads_status = 0;
  uint8_t _this_current_pads_status = 0;
  _hunter_callback _this_function;
  const char* _this_class;
  uint8_t _this_pin_number;
  public :
  inputHunter() {
    if(!_hunter_exists) {
      _hunter_exists = true;
    }
  }
  void assign(const char* _class, _hunter_callback _function);
  void recognition();
};

//Assign pin class and callback function a inputHunter
void inputHunter::assign(const char* _class, _hunter_callback _function) {
  if(_hunter_exists && _pad_exists){
    this->_this_class = _class;
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode != OUTPUT && strcmp(_this_pin->_pin_class,this->_this_class) == 0){
          this->_this_global_pads_status = this->_this_global_pads_status + digitalRead(_this_pin->_pin_number);
      }
    }
    this->_this_function = _function;
  }
}

//Recognition input class status
void inputHunter::recognition() {
  if(_hunter_exists && _pad_exists){
    this->_this_current_pads_status = 0;
    bool _current_status;
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode != OUTPUT && strcmp(_this_pin->_pin_class,this->_this_class) == 0){
          _current_status = digitalRead(_this_pin->_pin_number);
          this->_this_current_pads_status = this->_this_current_pads_status + _current_status;
          if(_current_status != _this_pin->_pin_old_status) {
            this->_this_pin_number = _this_pin->_pin_number;
          }
      }
    }
    if(this->_this_current_pads_status != this->_this_global_pads_status) {
      this->_this_function(this->_this_pin_number);
    }
  }
}

//Class for doList
class doList {
  private :
  uint16_t _list_position = _lists_counter++;
  uint16_t _list_function_position_counter = 0;
  uint16_t _functions_counter = 0;
  public :
  doList() {

  }
  void addTask(_list_callback _function);
  void next();
  //operator "++"
  doList operator++(int) {
    this->next();
    return *this;
  }
};

// addTask Function : add tasks to the list
void doList::addTask(_list_callback _function){
   _lists *_new_function = new _lists;
   _new_function->_list_index = this->_list_position;
   _new_function->_function_callback = _function;
   _new_function->_functions_counter = this->_functions_counter++;
   if (_first_function == NULL){
    _first_function = _new_function;
   }else{
    _last_function->_next_function= _new_function;
   }
   _last_function = _new_function;
   if (!_list_exists){
      _list_exists = true;
   }
}

// next Function : go to the task in the next list 
void doList::next(){
  if (_list_exists){
    for (_lists *_this_function = _first_function; _this_function != NULL; _this_function = _this_function->_next_function){
      if (this->_list_position == _this_function->_list_index){
        if(_this_function->_functions_counter == this->_list_function_position_counter) {
          _this_function->_function_callback();
        }
      }
    }
    this->_list_function_position_counter++;
    if(this->_list_function_position_counter == this->_functions_counter) {
      this->_list_function_position_counter = 0;
    }
  }
}

// Pad class: initialize a pin of your board 
class Pad{
  private : 
  uint8_t _this_pin_number;
  uint8_t _this_pin_mode;
  public :
  Pad(uint8_t _pin_number, uint8_t _pin_mode = OUTPUT, const char *_pin_class = "nope", uint8_t _pin_start_value = 0, uint8_t _pin_min_value = 0, uint8_t _pin_max_value = 255) {
    _this_pin_number = _pin_number;
    if(_pin_mode != PWM_OUTPUT) {
      // If the pin is set in INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT mode
      _this_pin_mode = _pin_mode;
      _pins *_new_pin = new _pins;
      _new_pin->_pin_class = _pin_class;
      _new_pin->_pin_number = _pin_number;
      _new_pin->_pin_status = _pin_start_value;
      _new_pin->_pin_mode = _pin_mode;
      _new_pin->_pin_locked = UNLOCK;
      if (_first_pin == NULL){
        _first_pin = _new_pin;
      }else{
        _last_pin->_next_pin = _new_pin;
      }
      if (_pin_mode != OUTPUT){
        _new_pin->_pin_previous_status = _pin_start_value;
        _new_pin->_pin_debounce_delay_millis = _pin_button_default_debounce_millis;
        _new_pin->_pin_debounced_status = 0;
        _new_pin->_pin_switch_status = _pin_start_value;
        _new_pin->_pin_switch_release_button = 1;
        _new_pin->_pin_old_status = digitalRead(_new_pin->_pin_number);
      }
      pinMode(_pin_number, _pin_mode);
      if (_pin_mode == OUTPUT){
        digitalWrite(_pin_number, _pin_start_value);
      }
      _last_pin = _new_pin;
      if (!_pad_exists){
        _pad_exists = true;
      }
    }
    if(_pin_mode == PWM_OUTPUT) {
      // If the pin is set in PWM_OUTPUT mode 
      _this_pin_mode = PWM_OUTPUT;
      _pwm_pins *_new_pwm_pin = new _pwm_pins;
      _new_pwm_pin->_pwm_pin_number = _pin_number;
      _new_pwm_pin->_pwm_pin_locked = UNLOCK;
      if (_first_pwm_pin == NULL){
        _first_pwm_pin = _new_pwm_pin;
      }else{
      _last_pwm_pin->_next_pwm_pin = _new_pwm_pin;
      }
      pinMode(_pin_number, OUTPUT);
      _last_pwm_pin = _new_pwm_pin;
      _new_pwm_pin->_pwm_pin_class = _pin_class;
      _new_pwm_pin->_pwm_pin_value = _pin_start_value;
      _new_pwm_pin->_pwm_min_value = _pin_min_value;
      _new_pwm_pin->_pwm_max_value = _pin_max_value;
      _new_pwm_pin->_pwm_pin_fade_direction = false;
      _new_pwm_pin->_pwm_pin_enabled = true;
      _new_pwm_pin->_fade_effect = false;
      if (!_pwm_pad_exists){
       _pwm_pad_exists = true;
      }
    }
    // If the pin is set to ANALOG_INPUT mode, no change is made to the pin 
  }
  // Definition of the functions of the Pad Class
  void on();
  void off();
  void toggle();
  void lock();
  void unlock();
  void write(uint8_t _value);
  uint16_t read();
  uint8_t pinNumber();
  //operator "="
  Pad operator=(const uint8_t _new_value) {
    this->write(_new_value);
    return *this;
  }
};

// Pad Class on Function: Turns on a digital pin 
void Pad::on() {
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_number == this->_this_pin_number && _this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_status = HIGH; 
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status); 
      }
    }
  }
}

// Pad Class off Function : Turns off a digital pin
void Pad::off() {
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_number == this->_this_pin_number && _this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_status = LOW; 
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status); 
      }
    }
  }
}

// Pad Class off Function : Turns off a digital pin
void Pad::lock() {
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_number == this->_this_pin_number && _this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_locked = LOCK; 
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status); 
      }
    }
  }
}

// Pad Class off Function : Turns off a digital pin
void Pad::unlock() {
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_number == this->_this_pin_number && _this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_locked = UNLOCK;  
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status); 
      }
    }
  }
}

// Pad Class Toggle Function: Inverts the value of a digital pin 
void Pad::toggle() {
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_number == this->_this_pin_number && _this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_status = !_this_pin->_pin_status;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// Pad Class write Function: Assigns a value to a pin, if digital (0 and 1), if PWM (0 to 255) 
void Pad::write(uint8_t _new_value) {
  if(this->_this_pin_mode != PWM_OUTPUT) {
  // If the pin is not a pwm output 
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_number == this->_this_pin_number && _this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_status = _new_value;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
  }
  if(this->_this_pin_mode == PWM_OUTPUT){
  // If the pin is a pwm output 
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_number == this->_this_pin_number){
        _this_pwm_pin->_pwm_pin_value = _new_value;
        analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
      }
    }
  }
  }
}

// Pad Class read Function: Reads the value of a digital or analog pin 
uint16_t Pad::read() {
  if(this->_this_pin_mode != ANALOG_INPUT) {
    // If the pin is not an analog input 
    if (_pad_exists){
      for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
        if (_this_pin->_pin_number == this->_this_pin_number){
          return digitalRead(this->_this_pin_number);
        }
      }
    }
  }
  if(this->_this_pin_mode == ANALOG_INPUT) {
    // If the pin is an analog input 
    return analogRead(this->_this_pin_number);
  }
  if(this->_this_pin_mode == PWM_OUTPUT) {
    // If the pin is an pwm output 
    if (_pwm_pad_exists){
      for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
        if (_this_pwm_pin->_pwm_pin_number == this->_this_pin_number){
          return _this_pwm_pin->_pwm_pin_value;
        }
      }
    }
  }
  return 0;
}

// Pad Class pinNumber Function : Returns the current pin number 
uint8_t Pad::pinNumber() {
  if (_pad_exists || _pwm_pad_exists){
    // Returns the pin number 
    return this->_this_pin_number;
  }
  return 0;
}

// TickTimer Class
class TickTimer {
  private : 
  unsigned long _this_position = _ticks_counter++;
  public :
  TickTimer() {
    _ticks *_new_tick = new _ticks;
    if (_first_tick == NULL){
    _first_tick = _new_tick;
    }else{
    _last_tick->_next_tick = _new_tick;
    }
    _new_tick->_tick_position = _this_position;
    _new_tick->_tick_enabled = 1;
    _new_tick->_tick_previous_time = 0;
    _last_tick = _new_tick;
    if (!_ticks_exists){
      _ticks_exists = true;
    }
  }
  void attach(unsigned long _new_delay, _tick_callback _callback);
  void play();
  void pause();
};

// Tick Class Attach Function: attach a function to the timer
void TickTimer::attach(unsigned long _new_delay, _tick_callback _new_callback) {
  if (_ticks_exists){
    for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
      if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_delay = _new_delay;
          _this_tick->_tick_callback_function = _new_callback;
      }
    }
  }
}

// Tick Class Start Function: active a tick and starts it from a pause state
void TickTimer::play() {
  if (_ticks_exists){
    for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
      if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_enabled = 1;
      }
    }
  }
}

// Tick Class Pause Function: set pause state for a running tick
void TickTimer::pause() {
  if (_ticks_exists){
    for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
      if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_enabled = 0;
      }
    }
  }
}

// encoderAttach: attach an encoder 
void encoderAttach(uint8_t _new_encoder_dt_pin, uint8_t _new_encoder_clk_pin, _encoder_callback _new_encoder_change_callback){
  _encoders *_new_encoder = new _encoders;
  _new_encoder->_encoder_dt_pin = _new_encoder_dt_pin;
  _new_encoder->_encoder_clk_pin = _new_encoder_clk_pin;
  if (_first_encoder == NULL){
    _first_encoder = _new_encoder;
  }else{
    _last_encoder->_next_encoder = _new_encoder;
  }
  _new_encoder->_encoder_change_callback = _new_encoder_change_callback;
  _new_encoder->_encoder_debounce_delay_millis = _pin_encoder_default_debounce_millis;
  _last_encoder = _new_encoder;
  if (!_encoder_exists){
    _encoder_exists = true;
  }
}

// digitalLockAll: protects all output pins from use of digital functions
void digitalLockAll(){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_locked = LOCK;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// digitalUnlockAll: removes protection from all output pins for use of digital functions 
void digitalUnlockAll(){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode == OUTPUT){
        _this_pin->_pin_locked = UNLOCK;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// digitalToggleAll: toggle the state of all digital pins
void digitalToggleAll(){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_locked == UNLOCK){
        _this_pin->_pin_status = !_this_pin->_pin_status;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// digitalToggleClass: toggle the state to class of pins
void digitalToggleClass(const char *_digital_pin_class){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode == OUTPUT && strcmp(_this_pin->_pin_class,_digital_pin_class) == 0 && _this_pin->_pin_locked == UNLOCK){
        _this_pin->_pin_status = !_this_pin->_pin_status;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// digitalWriteAll: set all digital pins to a value
void digitalWriteAll(uint8_t _digital_status){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_status != _digital_status && _this_pin->_pin_locked == UNLOCK){
        _this_pin->_pin_status = _digital_status;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// digitalWriteClass: set a class of digital pins to a value
void digitalWriteClass(const char *_digital_pin_class, uint8_t _digital_status){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_status != _digital_status && strcmp(_this_pin->_pin_class,_digital_pin_class) == 0 && _this_pin->_pin_locked == UNLOCK){
        _this_pin->_pin_status = _digital_status;
        digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
      }
    }
  }
}

// analogLockAll: protects all output pins from use of analog functions
void analogLockAll(){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_enabled){
        _this_pwm_pin->_pwm_pin_locked = LOCK;
        analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
      }
    }
  }
}

// analogUnlockAll: removes protection from all output pins for use of analog functions 
void analogUnlockAll(){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_enabled){
        _this_pwm_pin->_pwm_pin_locked = UNLOCK;
        analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
      }
    }
  }
}

// analogWriteAll: set all pwm pins to value
void analogWriteAll(unsigned int _analog_status){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_enabled && _this_pwm_pin->_pwm_pin_value != _analog_status && _this_pwm_pin->_pwm_pin_locked == UNLOCK){
        _this_pwm_pin->_pwm_pin_value = _analog_status;
        analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
      }
    }
  }
}

// analogAttach: attach a pin to the Tweakly core
void analogAttach(uint8_t _pwm_pin_number, unsigned int _pwm_pin_value = 0){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_number == _pwm_pin_number && !_this_pwm_pin->_pwm_pin_enabled){
        _this_pwm_pin->_pwm_pin_value = _pwm_pin_value;
        _this_pwm_pin->_pwm_pin_enabled = true;
      }
    }
  }
}

// analogDetach: detach a pin from the Tweakly core
void analogDetach(uint8_t _pwm_pin_number){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_number == _pwm_pin_number && _this_pwm_pin->_pwm_pin_enabled){
        _this_pwm_pin->_pwm_pin_enabled = false;
      }
    }
  }
}

// analogWriteClass: set a value to a class of pins
void analogWriteClass(const char *_pwm_pin_class, unsigned int _analog_status){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_enabled && _this_pwm_pin->_pwm_pin_value != _analog_status && strcmp(_this_pwm_pin->_pwm_pin_class,_pwm_pin_class) == 0 && _this_pwm_pin->_pwm_pin_locked == UNLOCK){
        _this_pwm_pin->_pwm_pin_value = _analog_status;
        analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
      }
    }
  }
}

// analogWriteProgressive: apply progressive effect to PWM pin; ALWAYS -> pulsing IN -> rising OUT -> falling
void analogWriteProgressive(uint8_t _pwn_pin_number, unsigned long _pwm_delay, uint8_t _pwm_fade_mode){
  if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_enabled == true && _this_pwm_pin->_pwm_pin_number == _pwn_pin_number && _this_pwm_pin->_pwm_pin_locked == UNLOCK){
         if(_pwm_fade_mode == HIGH_TO_EDGE || _pwm_fade_mode == LOW_TO_EDGE || _pwm_fade_mode == PULSE_TO_EDGE) {
           _pwm_delay = _pwm_delay / _this_pwm_pin->_pwm_max_value;
           if(_pwm_fade_mode == HIGH_TO_EDGE) {
             _pwm_fade_mode = TO_HIGH;
           }
           if(_pwm_fade_mode == LOW_TO_EDGE) {
             _pwm_fade_mode = TO_LOW;
           }
           if(_pwm_fade_mode == PULSE_TO_EDGE) {
             _pwm_fade_mode = TO_PULSE;
           }
         }
         _this_pwm_pin->_pwm_fade_delay = _pwm_delay;
         _this_pwm_pin->_pwm_fade_mode = _pwm_fade_mode;
         _this_pwm_pin->_fade_effect = true;
         analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
      }
    }
  }
}

// digitalPushButton: get the state of a pushbutton (debounced)
bool digitalPushButton(uint8_t _digital_pin){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode != OUTPUT){
        if (_this_pin->_pin_number == _digital_pin){
          return !_this_pin->_pin_switch_release_button;
        }
      }
    }
  }
  return 0;
}

// digitalSwitchButton: get the state of a virtual switch attached to a physical momentary button (debounced)
bool digitalSwitchButton(uint8_t _digital_pin){
  if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode != OUTPUT){
        if (_this_pin->_pin_number == _digital_pin && _this_pin->_pin_switch_release_button == 1){
          return _this_pin->_pin_switch_status;
        }
      }
    }
  }
  return 0;
}

// TweaklyRun: run the core; must be called in loop
void TweaklyRun(){
  unsigned long _current_millis = millis();
  if (!_tweakly_ready){
    if (_ticks_exists){
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
          _this_tick->_tick_previous_time = _current_millis;
      }
    }
    if (_pad_exists){
    for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
      if (_this_pin->_pin_mode != OUTPUT){
          _this_pin->_pin_debounce_previous_millis = _current_millis;
        }
      }
    }
    if (_pwm_pad_exists){
      for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
        if (_this_pwm_pin->_pwm_pin_enabled){
          _this_pwm_pin->_pwm_pin_delay_current_millis = _current_millis;
        }
      }
    }
    if (_encoder_exists){
      for (_encoders *_this_encoder = _first_encoder; _this_encoder != NULL; _this_encoder = _this_encoder->_next_encoder){
        _this_encoder->_encoder_debounce_previous_millis = _current_millis;
        _this_encoder->_encoder_clk_pin_previous_status = digitalRead(_this_encoder->_encoder_clk_pin);
      }
    }
    _tweakly_ready = true;
  }else{
    if (_ticks_exists){
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
        if (_this_tick->_tick_enabled){
          _this_tick->_tick_current_millis = _current_millis;
          if ((unsigned long)(_this_tick->_tick_current_millis - _this_tick->_tick_previous_time) >= _this_tick->_tick_delay){
            _this_tick->_tick_previous_time = _this_tick->_tick_current_millis;
            _this_tick->_tick_callback_function();
          }
        }
      }
    }
    if (_pad_exists){
      for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin){
        _this_pin->_pin_debounce_current_millis = _current_millis;
        if (_this_pin->_pin_mode != OUTPUT){
          _this_pin->_pin_status = digitalRead(_this_pin->_pin_number);
          if (_this_pin->_pin_status != _this_pin->_pin_previous_status){
            _this_pin->_pin_debounce_previous_millis = _this_pin->_pin_debounce_current_millis;
            _this_pin->_pin_switch_release_button = 1;
          }
          if ((unsigned long)(_this_pin->_pin_debounce_current_millis - _this_pin->_pin_debounce_previous_millis) >= _this_pin->_pin_debounce_delay_millis &&
              _this_pin->_pin_status == _this_pin->_pin_previous_status){
            _this_pin->_pin_debounce_previous_millis = _this_pin->_pin_debounce_current_millis;
            _this_pin->_pin_debounced_status = _this_pin->_pin_status;
            if (_this_pin->_pin_switch_release_button == 1){
               _this_pin->_pin_switch_release_button = 0;
               _this_pin->_pin_switch_status = !_this_pin->_pin_switch_status;
            }
          }
        }
      }
    }
    if (_pwm_pad_exists){
    for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin){
      if (_this_pwm_pin->_pwm_pin_enabled == true && _this_pwm_pin->_fade_effect == true){
        _this_pwm_pin->_pwm_pin_delay_current_millis = _current_millis;
        if ((unsigned long)(_this_pwm_pin->_pwm_pin_delay_current_millis - _this_pwm_pin->_pwm_pin_delay_previous_millis) >= _this_pwm_pin->_pwm_fade_delay){
          switch (_this_pwm_pin->_pwm_fade_mode){
            case TO_PULSE:
              if (!_this_pwm_pin->_pwm_pin_fade_direction){
                _this_pwm_pin->_pwm_pin_value++;
                if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_max_value){
                  _this_pwm_pin->_pwm_pin_fade_direction = true;
                }
              }
              if (_this_pwm_pin->_pwm_pin_fade_direction){
                _this_pwm_pin->_pwm_pin_value--;
                if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_min_value){
                  _this_pwm_pin->_pwm_pin_fade_direction = false;
                }
              }
              break;
            case TO_HIGH:
              _this_pwm_pin->_pwm_pin_value++;
              if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_max_value){
                _this_pwm_pin->_pwm_pin_value = _this_pwm_pin->_pwm_max_value;
                _this_pwm_pin->_pwm_pin_enabled = false;
              }
              break;
            case TO_LOW:
              _this_pwm_pin->_pwm_pin_value--;
              if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_min_value){
                _this_pwm_pin->_pwm_pin_value = _this_pwm_pin->_pwm_min_value;
                _this_pwm_pin->_pwm_pin_enabled = false;
              }
              break;
          }
          _this_pwm_pin->_pwm_pin_delay_previous_millis = _this_pwm_pin->_pwm_pin_delay_current_millis;
        }
      }
    }
  }
   if (_encoder_exists){
    for (_encoders *_this_encoder = _first_encoder; _this_encoder != NULL; _this_encoder = _this_encoder->_next_encoder){
      _this_encoder->_encoder_debounce_current_millis = _current_millis;
      if ((unsigned long)(_this_encoder->_encoder_debounce_current_millis - _this_encoder->_encoder_debounce_previous_millis) >= _this_encoder->_encoder_debounce_delay_millis){
        _this_encoder->_encoder_clk_pin_status = digitalRead(_this_encoder->_encoder_clk_pin);
        _this_encoder->_encoder_debounce_previous_millis = _this_encoder->_encoder_debounce_current_millis;
        if (_this_encoder->_encoder_clk_pin_status != _this_encoder->_encoder_clk_pin_previous_status && _this_encoder->_encoder_clk_pin_status == HIGH){
          _this_encoder->_encoder_dt_pin_status = digitalRead(_this_encoder->_encoder_dt_pin);
          _this_encoder->_encoder_change_callback(_this_encoder->_encoder_dt_pin_status);
        }
        _this_encoder->_encoder_clk_pin_previous_status = _this_encoder->_encoder_clk_pin_status;
      }
    }
  }
  }
}

// Comments added by G.Bruno (@gbr1) on May 28th 2021

#endif
