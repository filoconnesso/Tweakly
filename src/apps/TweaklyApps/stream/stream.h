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
 * STREAM MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef STREAM_H
#define STREAM_H

namespace stream {

    // Class for Echo : class that creates an Echo object
    class Echo {
      private :
      Stream* _current_stream;
      public :
      Echo() {

      }
      void print(const char* phrase, ...);
      void write(const char* phrase, ...);
      void attach(Stream*);
    };

    // Echo Class print function : print on a Stream object

    void Echo::print(const char* phrase, ...) {
      char complete_phrase[200];
      va_list arguments;
      va_start(arguments, phrase);
      vsprintf(complete_phrase, phrase, arguments);
      va_end(arguments);
      this->_current_stream->print(complete_phrase);
    }

    // Echo Class attach function : attach a Stream object

    void Echo::attach(Stream* _new_stream) {
       this->_current_stream = _new_stream;
    } 

}

#endif