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
      double _current_value;
      bool _direction = false;
      double _start;
      double _end;
      bool _enabled = false;
      double _resolution;
      public :
      Pong() {}
      double value();
      void setRange(double _new_start, double _new_end, double _new_resolution);
    };

    double Pong::value() {
      if(_enabled) {
        if(_current_value == _start) {
          _direction = true;
        }
        if(_current_value == _end) {
          _direction = false;
        }
        if(!_direction) {
          _current_value -= _resolution;
        } else if(_direction) {
          _current_value += _resolution;
        }
      }
      return _current_value;
    } 

    void Pong::setRange(double _new_start, double _new_end, double _new_resolution = 1.0) {
      _current_value = _new_start;
      _resolution = _new_resolution;
      _start = _new_start;
      _end = _new_end;
      _enabled = true;
    }

}

#endif