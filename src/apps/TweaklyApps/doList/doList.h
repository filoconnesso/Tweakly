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
 * DOLIST MODULE FOR TWEAKLY
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef DOLIST_H
#define DOLIST_H

namespace dolist {

   //Variables
   unsigned long _lists_counter = 0;
   
   //Type definition 
   typedef void (*_list_callback)();

   //Enablers
   volatile bool _list_exists  { false };
   
   // Struct required for lists
   struct _lists{
     uint16_t       _list_index;
     uint16_t       _functions_counter;
     _list_callback _function_callback;
     _lists *       _next_function = NULL;
   };

   _lists    *_first_function =   NULL, *_last_function = NULL;

   //Class for doList
   class doList {
     private :
     uint16_t _list_position = _lists_counter++;
     uint16_t _list_function_position_counter = 0;
     uint16_t _functions_counter = 0;
     public :
     doList() {

     }
     void addTask(_list_callback _function);
     void next();
     //operator "++"
     doList operator++(int) {
       this->next();
       return *this;
     }
   };

   // addTask Function : add tasks to the list
   void doList::addTask(_list_callback _function){
     _lists *_new_function = new _lists;
     _new_function->_list_index = this->_list_position;
     _new_function->_function_callback = _function;
     _new_function->_functions_counter = this->_functions_counter++;
     if (_first_function == NULL){
       _first_function = _new_function;
     }else{
       _last_function->_next_function= _new_function;
     }
     _last_function = _new_function;
     if (!_list_exists){
      _list_exists = true;
     }
    }

    // next Function : go to the task in the next list 
    void doList::next(){
       if (_list_exists){
         for (_lists *_this_function = _first_function; _this_function != NULL; _this_function = _this_function->_next_function){
          if (this->_list_position == _this_function->_list_index){
            if(_this_function->_functions_counter == this->_list_function_position_counter) {
              _this_function->_function_callback();
            }
           }
          }
         this->_list_function_position_counter++;
         if(this->_list_function_position_counter >= this->_functions_counter) {
             this->_list_function_position_counter = 0;
         }
      }
    }

}

#endif