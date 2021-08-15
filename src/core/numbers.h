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
 * NUMBERS MODULE FOR TWEAKLY
 *
 *  
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef NUMBERS_H
#define NUMBERS_H

namespace tweaklynumbers {
    
    //Pong Class :
    class Pong {
      private :
      float _current_value = 0;
      bool _direction = false;
      float _start;
      float _end;
      bool _enabled = false;
      public :
      Pong() {}
      float value();
      void setRange(float _new_start, float _new_end);
    };

    float Pong::value() {
      if(_enabled) {
        if(_current_value == _start) {
          _direction = true;
        }
        if(_current_value == _end) {
          _direction = false;
        }
        if(!_direction) {
          _current_value--;
        } else if(_direction) {
          _current_value++;
        }
      }
      return _current_value;
    } 

    void Pong::setRange(float _new_start, float _new_end) {
      _start = _new_start;
      _end = _new_end;
      _enabled = true;
    }

}

#endif