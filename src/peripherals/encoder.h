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
 * ENCODERS MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef ENCODER_H
#define ENCODER_H

namespace encoder {
   
   //Type definition
   typedef void (*_encoder_callback)(bool);

   // Enablers
   volatile bool _encoder_exists  { false };
   
   //Variables
   unsigned long _pad_encoder_default_debounce_millis = 1;

   // Struct required for encoders
   struct _encoders{
     uint8_t        _encoder_dt_pad;
     uint8_t        _encoder_clk_pad;
     bool           _encoder_dt_pad_status;
     bool           _encoder_clk_pad_status;
     bool           _encoder_clk_pad_previous_status;
     unsigned long  _encoder_debounce_current_millis;
     unsigned long  _encoder_debounce_previous_millis;
     unsigned long  _encoder_debounce_delay_millis;
     _encoder_callback _encoder_change_callback;
     _encoders *    _next_encoder = NULL;
   };

   _encoders *_first_encoder =    NULL, *_last_encoder =  NULL;
   
   // encoderAttach: attach an encoder 
   void encoderAttach(uint8_t _new_encoder_dt_pad, uint8_t _new_encoder_clk_pad, _encoder_callback _new_encoder_change_callback){
    _encoders *_new_encoder = new _encoders;
    _new_encoder->_encoder_dt_pad = _new_encoder_dt_pad;
    _new_encoder->_encoder_clk_pad = _new_encoder_clk_pad;
     if (_first_encoder == NULL){
       _first_encoder = _new_encoder;
     }else{
       _last_encoder->_next_encoder = _new_encoder;
     }
     _new_encoder->_encoder_change_callback = _new_encoder_change_callback;
     _new_encoder->_encoder_debounce_delay_millis = _pad_encoder_default_debounce_millis;
     _last_encoder = _new_encoder;
     if (!_encoder_exists){
       _encoder_exists = true;
    }
   }
   
   // Setup for all encoders
   void Setup() {
    if (_encoder_exists){
      unsigned long _current_millis = millis();
      for (_encoders *_this_encoder = _first_encoder; _this_encoder != NULL; _this_encoder = _this_encoder->_next_encoder){
        _this_encoder->_encoder_debounce_previous_millis = _current_millis;
        _this_encoder->_encoder_clk_pad_previous_status = digitalRead(_this_encoder->_encoder_clk_pad);
      }
    }
   }
   
   // Loop for all encoders
   void Loop() {
    if (_encoder_exists){
      unsigned long _current_millis = millis();
      for (_encoders *_this_encoder = _first_encoder; _this_encoder != NULL; _this_encoder = _this_encoder->_next_encoder){
        _this_encoder->_encoder_debounce_current_millis = _current_millis;
        if ((unsigned long)(_this_encoder->_encoder_debounce_current_millis - _this_encoder->_encoder_debounce_previous_millis) >= _this_encoder->_encoder_debounce_delay_millis){
          _this_encoder->_encoder_clk_pad_status = digitalRead(_this_encoder->_encoder_clk_pad);
          _this_encoder->_encoder_debounce_previous_millis = _this_encoder->_encoder_debounce_current_millis;
          if (_this_encoder->_encoder_clk_pad_status != _this_encoder->_encoder_clk_pad_previous_status && _this_encoder->_encoder_clk_pad_status == HIGH){
            _this_encoder->_encoder_dt_pad_status = digitalRead(_this_encoder->_encoder_dt_pad);
            _this_encoder->_encoder_change_callback(_this_encoder->_encoder_dt_pad_status);
          }
          _this_encoder->_encoder_clk_pad_previous_status = _this_encoder->_encoder_clk_pad_status;
        }
      }
    }
   }


}

#endif