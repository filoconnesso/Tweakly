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
 * CLOCK MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef CLOCK_H
#define CLOCK_H

namespace tweaklyclock {

   // Counters
   unsigned long _clock_counter = 0;
  
   // Enablers
   volatile bool _clocks_exists { false };

   // Type definition
   typedef void (*_alarm_callback)();

   // Struct required for clocks
   struct _clocks{
     unsigned long  _clock_position;
     unsigned long  _clock_current_millis;
     unsigned long  _clock_delay;
     unsigned long  _clock_previous_time;
     uint8_t        _clock_seconds;
     uint8_t        _clock_minutes;
     uint8_t        _clock_hours;
     uint8_t        _alarm_seconds;
     uint8_t        _alarm_minutes;
     uint8_t        _alarm_hours;
     unsigned long  _clock_epoch;
     _alarm_callback  _alarm_callback_function;
    _clocks *       _next_clock = NULL;
   };

   _clocks *_first_clock = NULL, *_last_clock =  NULL;
   
  // Clock Class
  class Clock {
    private :
    unsigned long _this_position = _clock_counter++;
    public :
    Clock() {
      _clocks *_new_clock = new _clocks;
      if (_first_clock == NULL){
      _first_clock = _new_clock;
      }else{
       _last_clock->_next_clock = _new_clock;
      }
      _new_clock->_clock_previous_time = 0;
      _new_clock->_clock_seconds = 0;
      _new_clock->_clock_minutes = 0;
      _new_clock->_clock_hours = 0;
      _new_clock->_clock_delay = 1000;
      _new_clock->_clock_position = _this_position;
      _last_clock = _new_clock;
      if (!_clocks_exists){
        _clocks_exists = true;
      }
    }
    void setSeconds(uint8_t _new_seconds);
    void setMinutes(uint8_t _new_minutes);
    void setHours(uint8_t _new_hours);
    uint8_t getSeconds();
    uint8_t getMinutes();
    uint8_t getHours();
    void setAlarm(uint8_t _alarm_hours, uint8_t _alarm_minutes, uint8_t _alarm_seconds, _alarm_callback _alarm_callback);
    void autoSync();
  };
  
  //Sync clock with computer time
  void Clock::autoSync() {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          int hours, minutes, seconds = 0;
          if (sscanf(__TIME__, "%d:%d:%d", &hours, &minutes, &seconds) != 3) {
            return;
          }
          _this_clock->_clock_hours = hours;
          _this_clock->_clock_minutes = minutes;
          _this_clock->_clock_seconds = seconds;
        }
      }
    }
  }

  // Get seconds
  uint8_t Clock::getSeconds() {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          return _this_clock->_clock_seconds;
        }
      }
    }
    return 0;
  }
  
  //Get minutes
  uint8_t Clock::getMinutes() {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          return _this_clock->_clock_minutes;
        }
      }
    }
    return 0;
  }

  //Get hours
  uint8_t Clock::getHours() {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          return _this_clock->_clock_hours;
        }
      }
    }
    return 0;
  }

   // Set seconds
  void Clock::setSeconds(uint8_t _new_seconds) {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          _this_clock->_clock_seconds = _new_seconds;
        }
      }
    }
  }
  
  //Set minutes
  void Clock::setMinutes(uint8_t _new_minutes) {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          _this_clock->_clock_minutes = _new_minutes;
        }
      }
    }
  }

  //Set hours
  void Clock::setHours(uint8_t _new_hours) {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          _this_clock->_clock_hours = _new_hours;
        }
      }
    }
  }

  //Set alarm
  void Clock::setAlarm(uint8_t _new_alarm_hours, uint8_t _new_alarm_minutes, uint8_t _new_alarm_seconds, _alarm_callback _new_alarm_callback) {
    if (_clocks_exists){
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        if (_this_clock->_clock_position == this->_this_position){
          _this_clock->_alarm_hours = _new_alarm_hours;
          _this_clock->_alarm_minutes = _new_alarm_minutes;
          _this_clock->_alarm_seconds = _new_alarm_seconds;
          _this_clock->_alarm_callback_function = _new_alarm_callback;
        }
      }
    }
  }

  // Setup all Clocks
  void Setup() {
     if (_clocks_exists){
       unsigned long _current_millis = millis();
       for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
          _this_clock->_clock_previous_time = _current_millis;
       }
     }
  }
   
   // Loop for all Clocks
   void Loop() {
     if (_clocks_exists){
      unsigned long _current_millis = millis();
      for (_clocks *_this_clock = _first_clock; _this_clock != NULL; _this_clock = _this_clock->_next_clock){
        _this_clock->_clock_current_millis = _current_millis;
         if ((unsigned long)(_this_clock->_clock_current_millis - _this_clock->_clock_previous_time) >= _this_clock->_clock_delay){
         _this_clock->_clock_previous_time = _this_clock->_clock_current_millis;
         _this_clock->_clock_seconds++;
         if(_this_clock->_clock_seconds == 60) {
           _this_clock->_clock_seconds = 0;
           _this_clock->_clock_minutes++;
         }
         if(_this_clock->_clock_minutes == 60) {
           _this_clock->_clock_minutes = 0;
           _this_clock->_clock_hours++;
         }
         if(_this_clock->_clock_hours > 23) {
           _this_clock->_clock_hours = 0;
         }
         if(_this_clock->_clock_hours == _this_clock->_alarm_hours && 
            _this_clock->_clock_minutes == _this_clock->_alarm_minutes &&
            _this_clock->_clock_seconds == _this_clock->_alarm_seconds) {
              _this_clock->_alarm_callback_function();
            }
         }
      }
    }
   }

}

#endif