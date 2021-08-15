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
 * SONARS MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef SONAR_H
#define SONAR_H

namespace sonar {
    
    // Definitions for sonar
    #define SONAR_MICROSECONDS 0
    #define SONAR_MILLISECONDS 1
    #define SONAR_CENTIMETERS 2
    #define SONAR_INCHES 3

    // Type definition 
    typedef void (*_sonar_callback)(unsigned long);
    
    // Enablers 
    volatile bool _sonar_exists  { false };

    // Struct required for sonar
    struct _sonars{
      uint8_t         _sonar_echo_pin;
      uint8_t         _sonar_trigger_pin;
      uint8_t         _sonar_progress_status;
      unsigned long   _sonar_current_micros;
      unsigned long   _sonar_previous_micros;
      unsigned long   _sonar_delay_micros;
      uint8_t         _sonar_return_mode;
      _sonar_callback _sonar_change_callback;
      _sonars *       _next_sonar = NULL;
    };

    _sonars   *_first_sonar =      NULL, *_last_sonar =  NULL;

    // sonarAttach: attach a sonar
    void sonarAttach(uint8_t _new_sonar_echo_pin, uint8_t _new_sonar_trigger_pin, _sonar_callback _new_sonar_change_callback, uint8_t _new_sonar_return_mode){
      _sonars *_new_sonar = new _sonars;
      _new_sonar->_sonar_echo_pin = _new_sonar_echo_pin;
      _new_sonar->_sonar_trigger_pin = _new_sonar_trigger_pin;
      if (_first_sonar == NULL){
        _first_sonar = _new_sonar;
      }else{
       _last_sonar->_next_sonar = _new_sonar;
      }
      _new_sonar->_sonar_change_callback = _new_sonar_change_callback;
      _new_sonar->_sonar_progress_status = 0;
      _new_sonar->_sonar_previous_micros = micros();
      _new_sonar->_sonar_return_mode = _new_sonar_return_mode;
      _last_sonar = _new_sonar;
      if (!_sonar_exists){
        _sonar_exists = true;
      }
    } 

    // Setup for all sonar
    void Setup() {
      if (_sonar_exists){
        unsigned long _current_micros = micros();
        for (_sonars *_this_sonar = _first_sonar; _this_sonar != NULL; _this_sonar = _this_sonar->_next_sonar){
        _this_sonar->_sonar_previous_micros = _current_micros;
        }
      }
    }
    
    // Loop for all sonar
    void Loop() {
      if (_sonar_exists){
        unsigned long _current_micros = micros();
        for (_sonars *_this_sonar = _first_sonar; _this_sonar != NULL; _this_sonar = _this_sonar->_next_sonar){
          _this_sonar->_sonar_current_micros = _current_micros;
          if(_this_sonar->_sonar_progress_status == 0) {
          digitalWrite(_this_sonar->_sonar_trigger_pin, LOW);
          _this_sonar->_sonar_delay_micros = 2;
            if ((unsigned long)(_this_sonar->_sonar_current_micros - _this_sonar->_sonar_previous_micros) >= _this_sonar->_sonar_delay_micros){
               digitalWrite(_this_sonar->_sonar_trigger_pin, HIGH);
               _this_sonar->_sonar_delay_micros = 10;
               _this_sonar->_sonar_previous_micros = _current_micros;
               _this_sonar->_sonar_progress_status = 1;
            }
        }
        if(_this_sonar->_sonar_progress_status == 1) {
           if ((unsigned long)(_this_sonar->_sonar_current_micros - _this_sonar->_sonar_previous_micros) >= _this_sonar->_sonar_delay_micros){
            digitalWrite(_this_sonar->_sonar_trigger_pin, LOW);
            _this_sonar->_sonar_previous_micros = _current_micros;
            _this_sonar->_sonar_progress_status = 2;
           }
        }
        if(_this_sonar->_sonar_progress_status == 2) {
           long micros;
             micros = pulseIn(_this_sonar->_sonar_echo_pin, HIGH);
             switch(_this_sonar->_sonar_return_mode) {
             case SONAR_MICROSECONDS : 
               _this_sonar->_sonar_change_callback(micros);
             break;
             case SONAR_MILLISECONDS : 
               long millis;
               millis = micros / 1000;
               _this_sonar->_sonar_change_callback(millis);
             break;
             case SONAR_CENTIMETERS : 
               long distance_cm;
               distance_cm = micros / 29 / 2;
               _this_sonar->_sonar_change_callback(distance_cm);
             break;
             case SONAR_INCHES :
               long distance_inches;
               distance_inches = micros / 74 / 2;
               _this_sonar->_sonar_change_callback(distance_inches);
             break;
           }
         _this_sonar->_sonar_progress_status = 0;
         }
       } 
     }
   }

}

#endif