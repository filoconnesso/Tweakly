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
 * INPUT HUNTERS MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef INPUTHUNTERS_H
#define INPUTHUNTERS_H

namespace inputhunters {

    // Type definition
    typedef void (*_hunter_callback)(int);

    //Enablers
    volatile bool _hunter_exists { false };

    //Class for inputHunters
    class inputHunter {
      private :
      uint8_t _this_global_pads_status = 0;
      uint8_t _this_current_pads_status = 0;
      _hunter_callback _this_function;
      const char* _this_class;
      uint8_t _this_pad_number;
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
       for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
         if (_this_pad->_pad_mode != OUTPUT && strcmp(_this_pad->_pad_class,this->_this_class) == 0){
          this->_this_global_pads_status = this->_this_global_pads_status + digitalRead(_this_pad->_pad_number);
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
        for (_pads *_this_pad = _first_pad; _this_pad != NULL; _this_pad = _this_pad->_next_pad){
          if (_this_pad->_pad_mode != OUTPUT && strcmp(_this_pad->_pad_class,this->_this_class) == 0){
            _current_status = digitalRead(_this_pad->_pad_number);
            this->_this_current_pads_status = this->_this_current_pads_status + _current_status;
            if(_current_status != _this_pad->_pad_old_status) {
              this->_this_pad_number = _this_pad->_pad_number;
            }
          }
        }
       if(this->_this_current_pads_status != this->_this_global_pads_status) {
         this->_this_function(this->_this_pad_number);
       }
      }
    }

}

#endif