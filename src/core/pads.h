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
 * PADS MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef TWEAKLYPADS_H
#define TWEAKLYPADS_H

namespace tweaklypads {

// Type definition
typedef void (*_pad_callback)();

// Variables
unsigned long _pad_button_default_debounce_millis  = 50;

// Pad event
#define CLICK 0
#define DOUBLE_CLICK 1
#define LONG_PRESS 2
#define RELEASE 3
#define TO_ON 4
#define TO_OFF 5

// Definitions for PWM and ANALOG pin
#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN 150
#endif

#define MELODY_OUTPUT 168
#define PWM_OUTPUT 169
#define ANALOG_INPUT 190

// Definitions for locked and unlocked pads
#define LOCK 1
#define UNLOCK 0

// Definitions for analogWriteFade
#define TO_LOW           1                // -> falling effect
#define TO_HIGH          2                // -> rising effect
#define TO_PULSE         3                // -> pulsing effect
#define LOW_TO_EDGE      4                // -> falling effect
#define HIGH_TO_EDGE     5                // -> rising effect
#define PULSE_TO_EDGE    6                // -> pulsing effect

// Enablers
volatile bool _pad_exists { false };
volatile bool _pwm_pad_exists { false };

// Struct required for pads
struct _pads{
  const char *   _pad_class;
  uint8_t        _pad_number;                               
  uint8_t        _pad_locked;                                                                     
  bool           _pad_status;
  bool           _pad_previous_status;
  bool           _pad_debounced_status;
  bool           _pad_debounced_start_status;
  bool           _pad_switch_status;
  bool           _pad_switch_release_button;
  bool           _pad_button_releasing;
  bool           _pad_output_to_on;
  bool           _pad_output_to_off;
  bool           _pad_old_status;
  bool           _pad_long_action;
  int            _pad_rapid_action_counter;
  unsigned long  _pad_rapid_action_time;
  uint8_t        _pad_mode;                           
  unsigned long  _pad_debounce_current_millis;
  unsigned long  _pad_debounce_previous_millis;
  unsigned long  _pad_debounce_delay_millis;
  _pad_callback _double_click_callback_function;
  _pad_callback _click_callback_function;
  _pad_callback _long_press_callback_function;
  _pad_callback _release_callback_function;
  _pad_callback _to_on_callback_function;
  _pad_callback _to_off_callback_function;
  _pads *        _next_pad = NULL;
};

// Struct required for pwm pads
struct _pwm_pads{
  const char *   _pwm_pad_class;
  uint8_t        _pwm_pad_number;
  uint8_t        _pwm_pad_locked;
  unsigned int   _pwm_pad_value;
  unsigned long  _pwm_pad_delay_current_millis;
  unsigned long  _pwm_pad_delay_previous_millis;
  bool           _pwm_pad_fade_direction;
  unsigned long  _pwm_max_value;
  unsigned long  _pwm_min_value;
  bool           _pwm_pad_enabled;
  uint8_t        _pwm_fade_mode;
  unsigned long  _pwm_fade_delay;
  bool           _fade_effect;
  _pwm_pads *    _next_pwm_pad = NULL;
};

_pads     *_first_pad =        NULL, *_last_pad =  NULL;
_pwm_pads *_first_pwm_pad =    NULL, *_last_pwm_pad =  NULL;

void nullCallback() {
  //null callback
}

// Pad class: initialize a pin of your board 
class Pad{
  private : 
  uint8_t _this_pad_number;
  uint8_t _this_pad_mode;
  public :
  Pad(uint8_t _pad_number, uint8_t _pad_mode = OUTPUT, const char *_pad_class = "nope", uint8_t _pad_start_value = 0, uint8_t _pad_min_value = 0, uint8_t _pad_max_value = 255) {
    _this_pad_number = _pad_number;
    if(_pad_mode != ANALOG_INPUT) {
      if(_pad_mode != PWM_OUTPUT) {
        // If the pin is set in INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT mode
        _this_pad_mode = _pad_mode;
        _pads *_new_pad = new _pads;
        _new_pad->_pad_class = _pad_class;
        _new_pad->_pad_number = _pad_number;
        _new_pad->_pad_status = _pad_start_value;
        _new_pad->_pad_mode = _pad_mode;
        _new_pad->_pad_button_releasing = false;
        _new_pad->_pad_locked = UNLOCK;
        if (_first_pad == NULL){
          _first_pad = _new_pad;
        }else{
          _last_pad->_next_pad = _new_pad;
        }
        if (_pad_mode != OUTPUT){
          _new_pad->_pad_previous_status = _pad_start_value;
          _new_pad->_pad_debounce_delay_millis = _pad_button_default_debounce_millis;
          _new_pad->_pad_debounced_status = 0;
          _new_pad->_pad_switch_status = !_pad_start_value;
          _new_pad->_pad_switch_release_button = 1;
          _new_pad->_pad_old_status = digitalRead(_new_pad->_pad_number);
            if(_pad_mode == INPUT || _pad_mode == INPUT_PULLUP || _pad_mode == INPUT_PULLDOWN) {
              _new_pad->_pad_rapid_action_time = millis();
              _new_pad->_click_callback_function = nullCallback;
              _new_pad->_release_callback_function = nullCallback;
              _new_pad->_double_click_callback_function = nullCallback;
              _new_pad->_long_press_callback_function = nullCallback;
            }
          pinMode(_pad_number, _pad_mode);
        }
        if (_pad_mode == OUTPUT){
          pinMode(_pad_number, _pad_mode);
          digitalWrite(_pad_number, _pad_start_value);
          if(_pad_start_value == 0) {
            _new_pad->_pad_output_to_off = true;
            _new_pad->_pad_output_to_on = false;
          } else {
            _new_pad->_pad_output_to_off = false;
            _new_pad->_pad_output_to_on = true;
          }
          _new_pad->_to_off_callback_function = nullCallback;
          _new_pad->_to_on_callback_function = nullCallback;
        }
        _last_pad = _new_pad;
        if (!_pad_exists){
          _pad_exists = true;
        }
      }
      if(_pad_mode == PWM_OUTPUT || _pad_mode == MELODY_OUTPUT) {
        // If the pin is set in PWM_OUTPUT mode 
        _this_pad_mode = PWM_OUTPUT;
        _pwm_pads *_new_pwm_pad = new _pwm_pads;
        _new_pwm_pad->_pwm_pad_number = _pad_number;
        _new_pwm_pad->_pwm_pad_locked = UNLOCK;
        if (_first_pwm_pad == NULL){
          _first_pwm_pad = _new_pwm_pad;
        }else{
        _last_pwm_pad->_next_pwm_pad = _new_pwm_pad;
        }
        pinMode(_pad_number, OUTPUT);
        _last_pwm_pad = _new_pwm_pad;
        _new_pwm_pad->_pwm_pad_class = _pad_class;
        _new_pwm_pad->_pwm_pad_value = _pad_start_value;
        _new_pwm_pad->_pwm_min_value = _pad_min_value;
        _new_pwm_pad->_pwm_max_value = _pad_max_value;
        _new_pwm_pad->_pwm_pad_fade_direction = false;
        _new_pwm_pad->_pwm_pad_enabled = true;
        _new_pwm_pad->_fade_effect = false;
        #if defined(ARDUINO_ARCH_ESP32)
        if(!esp32::_pad_is_exists(_new_pwm_pad->_pwm_pad_number)) {
	        esp32::_create_pad(_new_pwm_pad->_pwm_pad_number);
        }
        #endif
        #if defined(ARDUINO_SAM_DUE)
        if(_pad_mode == MELODY_OUTPUT) {
          if(!arduino_boards::_due_buzzer_pad_is_exists(_pad_number)) {
	          arduino_boards::_due_create_buzzer_pad(_pad_number);
          }
        }
        #endif
        if (!_pwm_pad_exists){
         _pwm_pad_exists = true;
        }
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
  unsigned long read();
  uint8_t pinNumber();
  void onEvent(uint8_t _event, _pad_callback _callback);

  //operator "="
  Pad operator=(const uint8_t _new_value) {
    this->write(_new_value);
    return *this;
  }

};


// Pad Class onEvent Function : Modern function for capturing events on a pin
void Pad::onEvent(uint8_t _event, _pad_callback _callback) {
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number){
        switch(_event) {
        case CLICK : 
          _this_pad->_click_callback_function = _callback; 
        break;
        case DOUBLE_CLICK : 
          _this_pad->_double_click_callback_function = _callback; 
        break;
        case LONG_PRESS : 
          _this_pad->_long_press_callback_function = _callback; 
        break;
        case RELEASE :
          _this_pad->_release_callback_function = _callback; 
        break;
        case TO_ON :
          _this_pad->_to_on_callback_function = _callback; 
        break;
        case TO_OFF :
          _this_pad->_to_off_callback_function = _callback; 
        break;
        }
      }
    }
  }
}

// Pad Class on Function: Turns on a digital pin 
void Pad::on() {
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number && _this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_status = HIGH; 
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status); 
      }
    }
  }
}

// Pad Class off Function : Turns off a digital pin
void Pad::off() {
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number && _this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_status = LOW; 
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status); 
      }
    }
  }
}

// Pad Class off Function : Turns off a digital pin
void Pad::lock() {
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number && _this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_locked = LOCK; 
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status); 
      }
    }
  }
}

// Pad Class off Function : Turns off a digital pin
void Pad::unlock() {
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number && _this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_locked = UNLOCK;  
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status); 
      }
    }
  }
}

// Pad Class Toggle Function: Inverts the value of a digital pin 
void Pad::toggle() {
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number && _this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_status = !_this_pad->_pad_status;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// Pad Class write Function: Assigns a value to a pin, if digital (0 and 1), if PWM (0 to 255) 
void Pad::write(uint8_t _new_value) {
  if(this->_this_pad_mode != PWM_OUTPUT) {
  // If the pin is not a pwm output 
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_number == this->_this_pad_number && _this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_status = _new_value;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
  }
  if(this->_this_pad_mode == PWM_OUTPUT){
  // If the pin is a pwm output 
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_number == this->_this_pad_number){
        _this_pwm_pad->_pwm_pad_enabled = true;
        _this_pwm_pad->_pwm_pad_value = _new_value;
        analogWrite(_this_pwm_pad->_pwm_pad_number, _this_pwm_pad->_pwm_pad_value);
      }
    }
  }
  }
}

// Pad Class read Function: Reads the value of a digital or analog pin 
uint16_t Pad::read() {
  unsigned long _pad_value = 0;
  if(this->_this_pad_mode != ANALOG_INPUT) {
    // If the pin is not an analog input 
    if (_pad_exists){
      for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
        if (_this_pad->_pad_number == this->_this_pad_number){
          _pad_value = digitalRead(this->_this_pad_number);
        }
      }
    }
  }
  if(this->_this_pad_mode == ANALOG_INPUT) {
    // If the pin is an analog input 
    _pad_value = analogRead(this->_this_pad_number);
  }
  if(this->_this_pad_mode == PWM_OUTPUT) {
    // If the pin is an pwm output 
    if (_pwm_pad_exists){
      for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
        if (_this_pwm_pad->_pwm_pad_number == this->_this_pad_number){
          _pad_value =  _this_pwm_pad->_pwm_pad_value;
        }
      }
    }
  }
  return _pad_value;
}

// Pad Class pinNumber Function : Returns the current pin number 
uint8_t Pad::pinNumber() {
  if (_pad_exists || _pwm_pad_exists){
    // Returns the pin number 
    return this->_this_pad_number;
  }
  return 0;
}

// digitalLockAll: protects all output pins from use of digital functions
void digitalLockAll(){
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_locked = LOCK;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// digitalUnlockAll: removes protection from all output pins for use of digital functions 
void digitalUnlockAll(){
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode == OUTPUT){
        _this_pad->_pad_locked = UNLOCK;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// digitalToggleAll: toggle the state of all digital pins
void digitalToggleAll(){
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode == OUTPUT && _this_pad->_pad_locked == UNLOCK){
        _this_pad->_pad_status = !_this_pad->_pad_status;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// digitalToggleClass: toggle the state to class of pins
void digitalToggleClass(const char *_digital_pad_class){
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode == OUTPUT && strcmp(_this_pad->_pad_class,_digital_pad_class) == 0 && _this_pad->_pad_locked == UNLOCK){
        _this_pad->_pad_status = !_this_pad->_pad_status;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// digitalWriteAll: set all digital pins to a value
void digitalWriteAll(uint8_t _digital_status){
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode == OUTPUT && _this_pad->_pad_status != _digital_status && _this_pad->_pad_locked == UNLOCK){
        _this_pad->_pad_status = _digital_status;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// digitalWriteClass: set a class of digital pins to a value
void digitalWriteClass(const char *_digital_pad_class, uint8_t _digital_status){
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode == OUTPUT && _this_pad->_pad_status != _digital_status && strcmp(_this_pad->_pad_class,_digital_pad_class) == 0 && _this_pad->_pad_locked == UNLOCK){
        _this_pad->_pad_status = _digital_status;
        digitalWrite(_this_pad->_pad_number, _this_pad->_pad_status);
      }
    }
  }
}

// analogLockAll: protects all output pins from use of analog functions
void analogLockAll(){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_enabled){
        _this_pwm_pad->_pwm_pad_locked = LOCK;
        analogWrite(_this_pwm_pad->_pwm_pad_number, _this_pwm_pad->_pwm_pad_value);
      }
    }
  }
}

// analogUnlockAll: removes protection from all output pins for use of analog functions 
void analogUnlockAll(){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_enabled){
        _this_pwm_pad->_pwm_pad_locked = UNLOCK;
        analogWrite(_this_pwm_pad->_pwm_pad_number, _this_pwm_pad->_pwm_pad_value);
      }
    }
  }
}

// analogWriteAll: set all pwm pins to value
void analogWriteAll(unsigned int _analog_status){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_enabled && _this_pwm_pad->_pwm_pad_value != _analog_status && _this_pwm_pad->_pwm_pad_locked == UNLOCK){
        _this_pwm_pad->_pwm_pad_value = _analog_status;
        analogWrite(_this_pwm_pad->_pwm_pad_number, _this_pwm_pad->_pwm_pad_value);
      }
    }
  }
}

// analogAttach: attach a pin to the Tweakly core
void analogAttach(uint8_t _pwm_pad_number, unsigned int _pwm_pad_value = 0){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_number == _pwm_pad_number && !_this_pwm_pad->_pwm_pad_enabled){
        _this_pwm_pad->_pwm_pad_value = _pwm_pad_value;
        _this_pwm_pad->_pwm_pad_enabled = true;
      }
    }
  }
}

// analogDetach: detach a pin from the Tweakly core
void analogDetach(uint8_t _pwm_pad_number){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_number == _pwm_pad_number && _this_pwm_pad->_pwm_pad_enabled){
        _this_pwm_pad->_pwm_pad_enabled = false;
      }
    }
  }
}

// analogWriteClass: set a value to a class of pins
void analogWriteClass(const char *_pwm_pad_class, unsigned int _analog_status){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_enabled && _this_pwm_pad->_pwm_pad_value != _analog_status && strcmp(_this_pwm_pad->_pwm_pad_class,_pwm_pad_class) == 0 && _this_pwm_pad->_pwm_pad_locked == UNLOCK){
        _this_pwm_pad->_pwm_pad_value = _analog_status;
        analogWrite(_this_pwm_pad->_pwm_pad_number, _this_pwm_pad->_pwm_pad_value);
      }
    }
  }
}

// analogWriteProgressive: apply progressive effect to PWM pin; ALWAYS -> pulsing IN -> rising OUT -> falling
void analogWriteProgressive(uint8_t _pwn_pad_number, unsigned long _pwm_delay, uint8_t _pwm_fade_mode){
  if (_pwm_pad_exists){
    for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
      if (_this_pwm_pad->_pwm_pad_enabled == true && _this_pwm_pad->_pwm_pad_number == _pwn_pad_number && _this_pwm_pad->_pwm_pad_locked == UNLOCK){
         if(_pwm_fade_mode == HIGH_TO_EDGE || _pwm_fade_mode == LOW_TO_EDGE || _pwm_fade_mode == PULSE_TO_EDGE) {
           _pwm_delay = _pwm_delay / _this_pwm_pad->_pwm_max_value;
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
         _this_pwm_pad->_pwm_fade_delay = _pwm_delay;
         _this_pwm_pad->_pwm_fade_mode = _pwm_fade_mode;
         _this_pwm_pad->_fade_effect = true;
         analogWrite(_this_pwm_pad->_pwm_pad_number, _this_pwm_pad->_pwm_pad_value);
      }
    }
  }
}

// digitalPushButton: get the state of a pushbutton (debounced)
bool digitalPushButton(uint8_t _digital_pad){
  bool _sw_status = 0;
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode != OUTPUT){
        if (_this_pad->_pad_number == _digital_pad){
          _sw_status = !_this_pad->_pad_switch_release_button;
        }
      }
    }
  }
  return _sw_status;
}

// digitalSwitchButton: get the state of a virtual switch attached to a physical momentary button (debounced)
bool digitalSwitchButton(uint8_t _digital_pad){
  bool _sw_status = 0;
  if (_pad_exists){
    for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode != OUTPUT){
        if (_this_pad->_pad_number == _digital_pad){
          _sw_status = _this_pad->_pad_switch_status;
        }
      }
    }
  }
  return _sw_status;
}

// Setup for all pads
void Setup() {
    if (_pad_exists){
      unsigned long _current_millis = millis();
      for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
      if (_this_pad->_pad_mode != OUTPUT){
          _this_pad->_pad_debounce_previous_millis = _current_millis;
        }
      }
    }
    if (_pwm_pad_exists){
      unsigned long _current_millis = millis();
      for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
        if (_this_pwm_pad->_pwm_pad_enabled){
          _this_pwm_pad->_pwm_pad_delay_current_millis = _current_millis;
        }
      }
    }
}

// Loop for all pads
void Loop() {
    if (_pad_exists){
      unsigned long _current_millis = millis();
      for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
        _this_pad->_pad_debounce_current_millis = _current_millis;
        if (_this_pad->_pad_mode != OUTPUT){
          _this_pad->_pad_status = digitalRead(_this_pad->_pad_number);
          if (_this_pad->_pad_status != _this_pad->_pad_previous_status){
            _this_pad->_pad_debounce_previous_millis = _this_pad->_pad_debounce_current_millis;
            _this_pad->_pad_switch_release_button = 1;
            if(_this_pad->_pad_rapid_action_counter == 1 && _current_millis >= _this_pad->_pad_rapid_action_time) {
              _this_pad->_click_callback_function();
              _this_pad->_pad_rapid_action_counter = 0;
              _this_pad->_pad_long_action = false;
            }
            if(_this_pad->_pad_rapid_action_counter == 2) {
              _this_pad->_double_click_callback_function();
              _this_pad->_pad_rapid_action_counter = 0;
              _this_pad->_pad_long_action = false;
            }
            if(_this_pad->_pad_button_releasing){
              _this_pad->_pad_button_releasing = false;
              _this_pad->_release_callback_function();
            }
          }
          if ((unsigned long)(_this_pad->_pad_debounce_current_millis - _this_pad->_pad_debounce_previous_millis) >= _this_pad->_pad_debounce_delay_millis 
             && _this_pad->_pad_status == _this_pad->_pad_previous_status){
            _this_pad->_pad_debounce_previous_millis = _this_pad->_pad_debounce_current_millis;
            _this_pad->_pad_debounced_status = _this_pad->_pad_status;
            if (_this_pad->_pad_switch_release_button == 1){
               _this_pad->_pad_switch_release_button = 0;
               _this_pad->_pad_switch_status = !_this_pad->_pad_switch_status;
               _this_pad->_pad_rapid_action_time = _current_millis + 200;
               _this_pad->_pad_rapid_action_counter++;
               _this_pad->_pad_long_action = true;
               _this_pad->_pad_button_releasing = true;
            }
          }
          if(_this_pad->_pad_long_action && _current_millis >= _this_pad->_pad_rapid_action_time + 1200 && _this_pad->_pad_button_releasing == true) {
            _this_pad->_pad_long_action = false;
            _this_pad->_long_press_callback_function();
          }
          if(_current_millis > _this_pad->_pad_rapid_action_time) {
            _this_pad->_pad_rapid_action_counter = 0;
          }
        } else {
          if(_this_pad->_pad_status == HIGH) {
            if(!_this_pad->_pad_output_to_on) {
              _this_pad->_pad_output_to_on = true;
              _this_pad->_pad_output_to_off = false;
              _this_pad->_to_on_callback_function();
            }
          }
          if(_this_pad->_pad_status == LOW) {
            if(!_this_pad->_pad_output_to_off) {
              _this_pad->_pad_output_to_on = false;
              _this_pad->_pad_output_to_off = true;
              _this_pad->_to_off_callback_function();
            }
          }
        }
      }
    }
    if (_pwm_pad_exists){
      unsigned long _current_millis = millis();
      for (_pwm_pads *_this_pwm_pad = _first_pwm_pad; _this_pwm_pad != NULL; _this_pwm_pad = _this_pwm_pad->_next_pwm_pad){
       if (_this_pwm_pad->_pwm_pad_enabled == true && _this_pwm_pad->_fade_effect == true){
        _this_pwm_pad->_pwm_pad_delay_current_millis = _current_millis;
        if ((unsigned long)(_this_pwm_pad->_pwm_pad_delay_current_millis - _this_pwm_pad->_pwm_pad_delay_previous_millis) >= _this_pwm_pad->_pwm_fade_delay){
          switch (_this_pwm_pad->_pwm_fade_mode){
            case TO_PULSE:
              if (!_this_pwm_pad->_pwm_pad_fade_direction){
                _this_pwm_pad->_pwm_pad_value++;
                if (_this_pwm_pad->_pwm_pad_value == _this_pwm_pad->_pwm_max_value){
                  _this_pwm_pad->_pwm_pad_fade_direction = true;
                }
              }
              if (_this_pwm_pad->_pwm_pad_fade_direction){
                _this_pwm_pad->_pwm_pad_value--;
                if (_this_pwm_pad->_pwm_pad_value == _this_pwm_pad->_pwm_min_value){
                  _this_pwm_pad->_pwm_pad_fade_direction = false;
                }
              }
              break;
            case TO_HIGH:
              _this_pwm_pad->_pwm_pad_value++;
              if (_this_pwm_pad->_pwm_pad_value >= _this_pwm_pad->_pwm_max_value){
                _this_pwm_pad->_pwm_pad_value = _this_pwm_pad->_pwm_max_value;
                _this_pwm_pad->_pwm_pad_enabled = false;
              }
              break;
            case TO_LOW:
              _this_pwm_pad->_pwm_pad_value--;
              if (_this_pwm_pad->_pwm_pad_value <= _this_pwm_pad->_pwm_min_value){
                _this_pwm_pad->_pwm_pad_value = _this_pwm_pad->_pwm_min_value;
                _this_pwm_pad->_pwm_pad_enabled = false;
              }
              break;
          }
          _this_pwm_pad->_pwm_pad_delay_previous_millis = _this_pwm_pad->_pwm_pad_delay_current_millis;
        }
      }
    }
  }
}

}

#endif 
