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
 * ARDUINO BOARDS MACHINES FILE FOR TWEAKLY V 0.1
 *
 * This ARDUINO BOARDS support file is an official machines support file from Tweakly.
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef ARDUINO_BOARDS_H
#define ARDUINO_BOARDS_H

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_MBED_PORTENTA) || defined(ARDUINO_ARCH_MBED_NANO) || defined(ARDUINO_ARCH_MBED)
#include <cstdarg>
#include <stdarg.h>
#endif

namespace arduino_boards
{

#if defined(ARDUINO_SAM_DUE)

#define DEFAULT_NOTE_DURATION 1
    
    //Enablers
    volatile bool _due_buzzer_pads_exists{false};
    
    //Struct for Arduino Due buzzer pads
    struct _due_buzzer_pads {
        unsigned long _prev_time = 0;
        unsigned long _cur_time = 0;
        bool _output = false;
        bool _released = true;
        uint8_t _pin;
        _due_buzzer_pads *_next_due_buzzer_pad = NULL;
    };
    
    _due_buzzer_pads *_first_due_buzzer_pad = NULL, *_last_due_buzzer_pad = NULL;
    
    //Function to create a new pad for buzzer
    void _due_create_buzzer_pad(uint8_t _pin) {
        _due_buzzer_pads *_new_due_buzzer_pad = new _due_buzzer_pads;
        _new_due_buzzer_pad->_pin = _pin;
        _new_due_buzzer_pad->_prev_time = micros();
        if (_first_due_buzzer_pad == NULL) {
            _first_due_buzzer_pad = _new_due_buzzer_pad;
        }
        else {
            _last_due_buzzer_pad->_next_due_buzzer_pad = _new_due_buzzer_pad;
        }
        _last_due_buzzer_pad = _new_due_buzzer_pad;
        _due_buzzer_pads_exists = true;
    }
    
    //Check if a pin has not already been assigned to buzzer
    bool _due_buzzer_pad_is_exists(uint8_t _pin)
    {
        bool _is_exists = false;
        for (_due_buzzer_pads *_this_due_buzzer_pad = _first_due_buzzer_pad; _this_due_buzzer_pad != NULL; _this_due_buzzer_pad = _this_due_buzzer_pad->_next_due_buzzer_pad) {
            if (!_is_exists) {
                if (_this_due_buzzer_pad->_pin == _pin) {
                    _is_exists = true;
                }
            }
        }
        return _is_exists;
    }
    
    //Tone function for Arduino Due
    void tone(uint8_t _pin, int _note) {
        if (_due_buzzer_pads_exists) {
            for (_due_buzzer_pads *_this_due_buzzer_pad = _first_due_buzzer_pad; _this_due_buzzer_pad != NULL; _this_due_buzzer_pad = _this_due_buzzer_pad->_next_due_buzzer_pad) {
                if (_this_due_buzzer_pad->_pin == _pin) {
                    if (_this_due_buzzer_pad->_released) {
                        _this_due_buzzer_pad->_released = false;
                        _this_due_buzzer_pad->_output = true;
                        _this_due_buzzer_pad->_prev_time = micros();
                    }
                    if (!_this_due_buzzer_pad->_released) {
                        _this_due_buzzer_pad->_cur_time = micros();
                        digitalWrite(_pin, _this_due_buzzer_pad->_output);
                        if (_this_due_buzzer_pad->_cur_time % 1000000L / _note / 2 == 0) {
                            _this_due_buzzer_pad->_output = false;
                            digitalWrite(_pin, _this_due_buzzer_pad->_output);
                            _this_due_buzzer_pad->_released = true;
                        }
                    }
                }
            }
        }
    }
    
    //noTone function for Arduino Due
    void noTone(uint8_t _pin) {
        if (_due_buzzer_pads_exists) {
            for (_due_buzzer_pads *_this_due_buzzer_pad = _first_due_buzzer_pad; _this_due_buzzer_pad != NULL; _this_due_buzzer_pad = _this_due_buzzer_pad->_next_due_buzzer_pad) {
                if (_this_due_buzzer_pad->_pin == _pin) {
                    _this_due_buzzer_pad->_released = true;
                    digitalWrite(_pin, 0);
                }
            }
        }
    }

#endif

}

#endif
