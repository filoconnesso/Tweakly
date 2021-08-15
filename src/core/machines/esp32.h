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
 *  MER_esp32_pwm_channelTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
 //----------------------------------------------------------------------------//

/*
 * ESP32 MACHINES FILE FOR TWEAKLY V 0.1
 *
 * This ESP32 support file is an official machines support file from Tweakly. 
 * The following file introduces the "Tone" and "analogWrite" functions normally 
 * not present on ESP32 and allows full compatibility of Tweakly on ESP32.
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef ESP32_H
#define ESP32_H

#include "esp32-hal-ledc.h"

namespace esp32 {

//Variables
unsigned int _esp32_channel_counter = 0;
unsigned int _esp32_channel_max = 16;

//Enablers
volatile bool _esp32_pwm_pad_exists { false };

//Struct for esp32 pwm pads
struct _esp32_pwm_pads{
    bool _attached;
	uint8_t _channel;
	unsigned long _frequency = 1500;
	unsigned int _resolution = 10;
	uint8_t _pin;
    _esp32_pwm_pads * _next_esp32_pad = NULL;
};

_esp32_pwm_pads *_first_esp32_pad = NULL, *_last_esp32_pad =  NULL;

//Function to create a new pad for esp32 and assign the first available PWM channel
void _create_pad(uint8_t _pin) {
	if(_esp32_channel_counter < _esp32_channel_max) {
	  _esp32_pwm_pads* _new_esp32_pad = new _esp32_pwm_pads;
	  _new_esp32_pad->_pin = _pin;	  
	  if(_esp32_channel_counter < _esp32_channel_max) {
		  _new_esp32_pad->_channel = _esp32_channel_counter;
		  _esp32_channel_counter++;
	  }
	  if (_first_esp32_pad == NULL){
          _first_esp32_pad = _new_esp32_pad;
      } else {
          _last_esp32_pad->_next_esp32_pad = _new_esp32_pad;
      }    
	  _new_esp32_pad->_attached = false;
	  ledcSetup(_new_esp32_pad->_channel, _new_esp32_pad->_frequency, _new_esp32_pad->_resolution);
	  _last_esp32_pad = _new_esp32_pad;
	  _esp32_pwm_pad_exists = true;
	}
}

//Check if a pin has not already been assigned to a channe
bool _pad_is_exists(uint8_t _pin) {
	bool _is_exists = false;
	for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		if(!_is_exists) {  
		    if(_this_esp32_pad->_pin == _pin) { 
		      _is_exists = true;
		    }
		}
	}
	return _is_exists;
}

//Configure frequency and resolution of a pin
void setEsp32PadConfiguration(uint8_t _pin, unsigned long _new_frequency, unsigned int _new_resolution) {
	if(_esp32_pwm_pad_exists) {
	   for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		   if(_this_esp32_pad->_pin == _pin) {
		      _this_esp32_pad->_frequency = _new_frequency;
		      _this_esp32_pad->_resolution = _new_resolution;
		      ledcSetup(_this_esp32_pad->_channel, _this_esp32_pad->_frequency, _this_esp32_pad->_resolution);
		   }
	   }
	}
}

//Get pin channel
uint8_t getEsp32PadChannel(uint8_t _pin) {
	uint8_t _channel = 0;
	if(_esp32_pwm_pad_exists) {
	   for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		    if(_this_esp32_pad->_pin == _pin) {
				_channel = _this_esp32_pad->_channel;
			}
	   }
	}
	return _channel;
}

//Get pin frequency
unsigned long getEsp32PadFrequency(uint8_t _pin) {
	unsigned long _frequency = 0;
	if(_esp32_pwm_pad_exists) {
	   for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		    if(_this_esp32_pad->_pin == _pin) {
				_frequency = _this_esp32_pad->_frequency;
			}
	   }
	}
	return _frequency;
}

//Get pin resolution
unsigned int getEsp32PadResolution(uint8_t _pin) {
    unsigned int _resolution = 0;
	if(_esp32_pwm_pad_exists) {
	   for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		    if(_this_esp32_pad->_pin == _pin) {
				_resolution =  _this_esp32_pad->_resolution;
			}
	   }
	}
	return _resolution;
}

//Tone function for ESP32
void tone(uint8_t _pin, int _note)
{   
	if(_esp32_pwm_pad_exists) {
	  for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		 if(_this_esp32_pad->_pin == _pin) { 
			if(!_this_esp32_pad->_attached) {
			   ledcAttachPin(_this_esp32_pad->_pin, _this_esp32_pad->_channel);
			   ledcWriteTone(_this_esp32_pad->_channel, _note);
	           _this_esp32_pad->_attached = true;
	        }
		} 
	  }
	}
}

//noTone function for ESP32
void noTone(uint8_t _pin)
{
   if(_esp32_pwm_pad_exists) {
     for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		if(_this_esp32_pad->_pin == _pin) { 
			if(_this_esp32_pad->_attached) {
                ledcDetachPin(_this_esp32_pad->_pin);
                ledcWrite(_this_esp32_pad->_channel, 0);
				_this_esp32_pad->_attached = false;
	        }
		  }
	  }
   }
}

//analogWrite function for ESP32
void analogWrite(uint8_t _pin, uint16_t _new_pwm_value) {
   if(_esp32_pwm_pad_exists) {
   for (_esp32_pwm_pads *_this_esp32_pad = _first_esp32_pad; _this_esp32_pad != NULL; _this_esp32_pad = _this_esp32_pad->_next_esp32_pad){
		if(_this_esp32_pad->_pin == _pin) { 
			if(!_this_esp32_pad->_attached) {
                ledcAttachPin(_pin, _this_esp32_pad->_channel);
                _this_esp32_pad->_attached = true;
	        } else {
				ledcWrite(_this_esp32_pad->_channel, _new_pwm_value);
			}
		 }
	  }
    }
}

}


#endif