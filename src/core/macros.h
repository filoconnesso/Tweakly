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
 * MACROS MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * April 7th 2022
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef TWEAKLYMACROS_H
#define TWEAKLYMACROS_H

namespace asyncer
{

  int _await_counter = 0;
  int _cur_await = 0;
  int _await_set_counter = 0;
  bool _async = false;

  #define TWEAKLY_MAX_AWAIT_SLEEP_TIMER 10

  #define TWEAKLY_ROLLUP                  \
          _async = true;                  \
          do {
  #define END_ROLLUP                      \
          break; } while(_async);

  struct _awaitDelayList {
    int _cur_index;
    unsigned long _cur_millis;
    unsigned long _cur_previous_millis;
    bool _set;
    bool _enabled;
  };

  struct _awaitDelayList _awaitDelayListArray[TWEAKLY_MAX_AWAIT_SLEEP_TIMER];

  bool awaitControl(int _timer, unsigned long _milliseconds) {
      if(!_awaitDelayListArray[_timer]._set) {
        _awaitDelayListArray[_timer]._set = true;
      }
      if(_timer <= _await_counter - 1) {
        bool _break = false;
        _awaitDelayListArray[_timer]._cur_millis = millis();
         if((unsigned long) _awaitDelayListArray[_timer]._cur_millis - _awaitDelayListArray[_timer]._cur_previous_millis >= _milliseconds) {
          _awaitDelayListArray[_timer]._cur_previous_millis = _awaitDelayListArray[_timer]._cur_millis;
          _break = false;
         } else {
          _break = true;
         }
        return _break;
      } else {
        return false;
      }
  }
  
  #define sleep_for(M)                                            \
          if( awaitControl( _cur_await, M ) && _async ){          \
            if( _cur_await < _await_counter ) {                   \
               _cur_await++;                                      \
            }                                                     \
            _async = false;                                       \
            break;                                                \
          } else {                                                \
            if( _cur_await < _await_counter ) {                   \
              _async = false;                                     \
              _cur_await++;                                       \
            }                                                     \
          }                                                       \
  

  void Setup() {
    for(int i = 0; i < TWEAKLY_MAX_AWAIT_SLEEP_TIMER; i++) {
      _awaitDelayListArray[i] = (_awaitDelayList) {_await_counter, millis(), millis(), false, false};
      _await_counter++;
    }
  }

  void Loop() {
    for(int i = 0; i < TWEAKLY_MAX_AWAIT_SLEEP_TIMER; i++) {
      if(_awaitDelayListArray[i]._set) {
        if(!_awaitDelayListArray[i]._enabled) {
         _awaitDelayListArray[i]._enabled = true;
         _await_set_counter++;
        }
      }
    }
    if(_cur_await == _await_set_counter) { 
      _cur_await = 0; 
    } 
  }

}

#endif
