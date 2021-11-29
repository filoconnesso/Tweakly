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
 * POLYPHONIC MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef POLYPHONIC_H
#define POLYPHONIC_H

namespace tweaklypolyphonic {
   
   //Notes
   #define NOTE_B0  31
   #define NOTE_C1  33
   #define NOTE_CS1 35
   #define NOTE_D1  37
   #define NOTE_DS1 39
   #define NOTE_E1  41
   #define NOTE_F1  44
   #define NOTE_FS1 46
   #define NOTE_G1  49
   #define NOTE_GS1 52
   #define NOTE_A1  55
   #define NOTE_AS1 58
   #define NOTE_B1  62
   #define NOTE_C2  65
   #define NOTE_CS2 69
   #define NOTE_D2  73
   #define NOTE_DS2 78
   #define NOTE_E2  82
   #define NOTE_F2  87
   #define NOTE_FS2 93
   #define NOTE_G2  98
   #define NOTE_GS2 104
   #define NOTE_A2  110
   #define NOTE_AS2 117
   #define NOTE_B2  123
   #define NOTE_C3  131
   #define NOTE_CS3 139
   #define NOTE_D3  147
   #define NOTE_DS3 156
   #define NOTE_E3  165
   #define NOTE_F3  175
   #define NOTE_FS3 185
   #define NOTE_G3  196
   #define NOTE_GS3 208
   #define NOTE_A3  220
   #define NOTE_AS3 233
   #define NOTE_B3  247
   #define NOTE_C4  262
   #define NOTE_CS4 277
   #define NOTE_D4  294
   #define NOTE_DS4 311
   #define NOTE_E4  330
   #define NOTE_F4  349
   #define NOTE_FS4 370
   #define NOTE_G4  392
   #define NOTE_GS4 415
   #define NOTE_A4  440
   #define NOTE_AS4 466
   #define NOTE_B4  494
   #define NOTE_C5  523
   #define NOTE_CS5 554
   #define NOTE_D5  587
   #define NOTE_DS5 622
   #define NOTE_E5  659
   #define NOTE_F5  698
   #define NOTE_FS5 740
   #define NOTE_G5  784
   #define NOTE_GS5 831
   #define NOTE_A5  880
   #define NOTE_AS5 932
   #define NOTE_B5  988
   #define NOTE_C6  1047
   #define NOTE_CS6 1109
   #define NOTE_D6  1175
   #define NOTE_DS6 1245
   #define NOTE_E6  1319
   #define NOTE_F6  1397
   #define NOTE_FS6 1480
   #define NOTE_G6  1568
   #define NOTE_GS6 1661
   #define NOTE_A6  1760
   #define NOTE_AS6 1865
   #define NOTE_B6  1976
   #define NOTE_C7  2093
   #define NOTE_CS7 2217
   #define NOTE_D7  2349
   #define NOTE_DS7 2489
   #define NOTE_E7  2637
   #define NOTE_F7  2794
   #define NOTE_FS7 2960
   #define NOTE_G7  3136
   #define NOTE_GS7 3322
   #define NOTE_A7  3520
   #define NOTE_AS7 3729
   #define NOTE_B7  3951
   #define NOTE_C8  4186
   #define NOTE_CS8 4435
   #define NOTE_D8  4699
   #define NOTE_DS8 4978
   #define NOTE_END 9999

   // Variables
   unsigned long _players_counter = 0;

   // Enablers
   volatile bool _player_exists { false };

   // Struct required for players

   struct _players{
     unsigned long  _player_current_millis;
     unsigned long  _player_notes_delay;
     unsigned long  _player_previous_millis;
     unsigned long  _player_note_duration;
     bool           _player_paused;
     bool           _player_output;
     uint8_t        _player_buzzer_pin;
     int            _player_melody_notes[256];
     int            _player_number_of_notes;
     uint8_t        _player_current_note;
     uint8_t        _player_position;
     bool           _player_repeat;
    _players *      _next_player = NULL;
   };

   _players *_first_player = NULL, *_last_player =  NULL;
   
  // Player Class
  class Player {
    private : 
    unsigned long _this_position = _players_counter++;
    public :
    Player() {
      _players *_new_player = new _players;
      if (_first_player == NULL){
      _first_player = _new_player;
      }else{
       _last_player->_next_player = _new_player;
      }
      _new_player->_player_paused = 0;
      _new_player->_player_previous_millis = millis();
      _new_player->_player_note_duration = 200;
      _new_player->_player_notes_delay = 100;
      _new_player->_player_output = true;
      _new_player->_player_paused = true;
      _new_player->_player_current_note = 0;
      _new_player->_player_position = this->_this_position;
      _last_player = _new_player;
      if (!_player_exists){
        _player_exists = true;
      }
    }
    void setNoteDuration(uint8_t _new_note_duration);
    void setNoteDelay(uint8_t _new_note_delay);
    void buzzerAttach(uint8_t _buzzer_pin);
    void melodyAttach(int _melody_notes[]);
    void repeat(bool _melody_repeat);
    void play();
    void pause();
    void stop();
  };

   // Setup note duration
   void Player::setNoteDuration(uint8_t _new_note_duration) {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_note_duration = _new_note_duration;
          }
       }
     }
   }

   // Setup notes delay
   void Player::setNoteDelay(uint8_t _new_notes_delay) {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_notes_delay = _new_notes_delay;
          }
       }
     }
   }

   // attach a buzzer
   void Player::buzzerAttach(uint8_t _buzzer_pin) {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_buzzer_pin = _buzzer_pin;
          }
       }
     }
   }

   // set melody notes
   void Player::melodyAttach(int _melody_notes[]) {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_number_of_notes = 0;
            for(int i = 0; _melody_notes[i] != NOTE_END ; i++) {
               if(_melody_notes[i] < 0) {
                 _melody_notes[i] = 0;
               } 
               _this_player->_player_melody_notes[i] = _melody_notes[i];
               _this_player->_player_number_of_notes++;
            }
          }
       }
     }
   }

   // play melody
   void Player::play() {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_paused = false;
          }
       }
     }
   }

   // repeat melody
   void Player::repeat(bool _melody_repeat) {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_repeat = _melody_repeat;
          }
       }
     }
   }

   // pause melody
   void Player::pause() {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_paused = true;
            noTone(_this_player->_player_buzzer_pin);
          }
       }
     }
   }

   // stop melody
   void Player::stop() {
     if (_player_exists){
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          if(_this_player->_player_position == this->_this_position) {
            _this_player->_player_current_note = 0;
            _this_player->_player_paused = true;
            noTone(_this_player->_player_buzzer_pin);
          }
       }
     }
   }

   // Setup all Players
   void Setup() {
     if (_player_exists){
       unsigned long _current_millis = millis();
       for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
          _this_player->_player_previous_millis = _current_millis;
       }
     }
   }
   
   // Loop for all Players
   void Loop() {
     if (_player_exists){
      unsigned long _current_millis = millis();
      for (_players *_this_player = _first_player; _this_player != NULL; _this_player = _this_player->_next_player){
        _this_player->_player_current_millis = _current_millis;
        if (!_this_player->_player_paused){
          if (_this_player->_player_output) {
            if ((unsigned long)(_this_player->_player_current_millis - _this_player->_player_previous_millis) >= _this_player->_player_note_duration) {
               noTone(_this_player->_player_buzzer_pin);
               _this_player->_player_previous_millis = _this_player->_player_current_millis;
               _this_player->_player_output = false;
            }
          } 
          if(!_this_player->_player_output) {
            if ((unsigned long)(_this_player->_player_current_millis - _this_player->_player_previous_millis) >= _this_player->_player_notes_delay) {
               tone(_this_player->_player_buzzer_pin, _this_player->_player_melody_notes[_this_player->_player_current_note]);
               _this_player->_player_current_note++;
               if (_this_player->_player_current_note >= _this_player->_player_number_of_notes) {
                 _this_player->_player_current_note = 0;
                 if(!_this_player->_player_repeat) {
                   noTone(_this_player->_player_buzzer_pin);
                   _this_player->_player_paused = true;
                 }
               } 
               _this_player->_player_previous_millis = _this_player->_player_current_millis;
               _this_player->_player_output = true;
             }
          }
        }
      }
    }
  }

}

#endif