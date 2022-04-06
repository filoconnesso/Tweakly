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
 * TICK TIMERS MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef TWEAKLYTICKTIMERS_H
#define TWEAKLYTICKTIMERS_H

namespace tweaklyticktimers {

   //Mode
   #define DISPATCH_FOREVER 0
   #define DISPATCH_ONCE 1
   #define DISPATCH_OFF 2
   
   //Definitions for tick timer priorities
   #define tweaklyPriorityLow_1 0            // -> disabled
   #define tweaklyPriorityLow_2 1            // -> enable low time level 2 (delay + 1 * 3)
   #define tweaklyPriorityLow_3 2            // -> enable low time level 3 (delay + 2 * 3)
   #define tweaklyPriorityLow_4 3            // -> enable low time level 4 (delay + 3 * 3)
   #define tweaklyPriorityLow_5 4            // -> enable low time level 5 (delay + 4 * 3)
   #define tweaklyPriorityMedium_1 5         // -> enable medium time level 1 (delay + 5 * 2)
   #define tweaklyPriorityMedium_2 6         // -> enable medium time level 2 (delay + 6 * 2)
   #define tweaklyPriorityMedium_3 7         // -> enable medium time level 3 (delay + 7 * 2)
   #define tweaklyPriorityMedium_4 8         // -> enable medium time level 4 (delay + 8 * 2)
   #define tweaklyPriorityHigh_1 9           // -> enable high time level 1 (delay + 9)
   #define tweaklyPriorityHigh_2 10          // -> enable high time level 2 (delay + 10)
   #define tweaklyPriorityHigh_3 11          // -> enable high time level 3 (delay + 11)
   #define tweaklyPriorityHigh_4 12          // -> enable high time level 4 (delay + 12)
   #define tweaklyPriorityHighPlus_1 13      // -> enable high plus level 1 (real time)
   #define tweaklyPriorityHighPlus_2 14      // -> enable high plus level 2 (real time)
   #define tweaklyPriorityHighPlus_3 15      // -> enable high plus level 3 (real time)

   // Variables
   unsigned long _ticks_counter = 0;

   // Enablers
   volatile bool _ticks_exists { false };

   // Priority counter for tick timers
   uint8_t _tweakly_priority_counter = 15;

   // Type definition
   typedef void (*_tick_callback)();

   // Struct required for ticks
   struct _ticks{
     unsigned long  _tick_current_millis;
     unsigned long  _tick_delay;
     unsigned long  _tick_previous_time;
     unsigned long  _tick_position;
     uint8_t        _tick_priority;
     bool           _tick_enabled;
     uint8_t        _tick_mode;
     unsigned long  _tick_start_watch_time;
     unsigned long  _tick_stop_watch_time;
    _tick_callback _tick_callback_function;
    _ticks *       _next_tick = NULL;
   };

   _ticks *_first_tick = NULL, *_last_tick =  NULL;
   
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
      _new_tick->_tick_priority = tweaklyPriorityHighPlus_3;
      _new_tick->_tick_position = _this_position;
      _new_tick->_tick_enabled = 1;
      _new_tick->_tick_previous_time = 0;
      _new_tick->_tick_mode = DISPATCH_FOREVER;
      _last_tick = _new_tick;
      if (!_ticks_exists){
        _ticks_exists = true;
      }
    }
    void attach(unsigned long _new_delay, _tick_callback _callback, uint8_t _new_mode);
    void setPriority(uint8_t _tick_priority);
    void setInterval(unsigned long _tick_interval);
    void dispatchNow();
    void kick();
    void play();
    void pause();
    void startWatch();
    void stopWatch();
    unsigned long getWatchTime();
  };

  // Tick Class startWatch Function : Set the current milliseconds of the timer for the start of the WatchTime
  void TickTimer::startWatch() {
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_start_watch_time = millis();
        }
      }
    } 
  }

  // Tick Class stopWatch Function : Set the current milliseconds of the timer for the end of WatchTime
  void TickTimer::stopWatch() {
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_stop_watch_time = millis();
        }
      }
    } 
  }

  // Tick Class getWatchTime : Calculate the elapsed time of the WatchTime
  unsigned long TickTimer::getWatchTime() {
    unsigned long _current_watch_time = 0;
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _current_watch_time = _this_tick->_tick_stop_watch_time - _this_tick->_tick_start_watch_time;
        }
      }
    } 
    return _current_watch_time;
  }

  // Tick Class Attach Function: attach a function to the timer
  void TickTimer::attach(unsigned long _new_delay, _tick_callback _new_callback, uint8_t _new_mode = DISPATCH_FOREVER) {
    if (_ticks_exists){
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
        if (_this_tick->_tick_position == this->_this_position){
          if(_new_mode == DISPATCH_OFF) {
            _this_tick->_tick_enabled = false;
          }
          _this_tick->_tick_delay = _new_delay;
          _this_tick->_tick_mode = _new_mode;
          _this_tick->_tick_callback_function = _new_callback;
        }
      }
    }
  }

  // Tick Class Set Priority Functions : set priority for a Tick
  void TickTimer::setPriority(uint8_t _tick_priority) {
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_priority = _tick_priority;
          if(_tick_priority < 5) {
            _this_tick->_tick_delay = _this_tick->_tick_delay + _tick_priority * 3;
          }
          if(_tick_priority < 9 && _tick_priority > 5) {
            _this_tick->_tick_delay = _this_tick->_tick_delay + _tick_priority * 2;
          }
          if(_tick_priority < 13 && _tick_priority > 9) {
            _this_tick->_tick_delay = _this_tick->_tick_delay + _tick_priority; 
          }
        }
      }
    } 
  }

  // Tick Class Set Interval Functions : set interval to Tick
  void TickTimer::setInterval(unsigned long _tick_interval) {
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_delay = _tick_interval;
        }
      }
    } 
  }

  // Tick Class Dispatch Now : run the timer now
  void TickTimer::dispatchNow() {
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_callback_function();
        }
      }
    } 
  }

  // Tick Class Kick : requests the attention of the timer and resets the counter
  void TickTimer::kick() {
    if(_ticks_exists) {
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
       if (_this_tick->_tick_position == this->_this_position){
          _this_tick->_tick_enabled = false;
          _this_tick->_tick_previous_time = millis();
          _this_tick->_tick_enabled = true;
        }
      }
    } 
  }

  // Tick Class Play Function: active a tick and starts it from a pause state
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

   // Setup all Tick Timers
   void Setup() {
     if (_ticks_exists){
       unsigned long _current_millis = millis();
       for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
          _this_tick->_tick_previous_time = _current_millis;
       }
     }
   }
   
   // Loop for all Tick Timers
   void Loop() {
     if (_ticks_exists){
      unsigned long _current_millis = millis();
      for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick){
        if (_this_tick->_tick_enabled){
          _this_tick->_tick_current_millis = _current_millis;
          if(_this_tick->_tick_priority == _tweakly_priority_counter) {
            if ((unsigned long)(_this_tick->_tick_current_millis - _this_tick->_tick_previous_time) >= _this_tick->_tick_delay){
              _this_tick->_tick_previous_time = _this_tick->_tick_current_millis;
              _this_tick->_tick_callback_function();
              if(_this_tick->_tick_mode == DISPATCH_ONCE || _this_tick->_tick_mode == DISPATCH_OFF) {
                _this_tick->_tick_enabled = false;
              }
            }
          }
          if(_this_tick == _last_tick) {
            _tweakly_priority_counter--;
          }
          if(_tweakly_priority_counter == 0) {
            _tweakly_priority_counter = 15;
          }
        }
      }
    }
   }

}

#endif