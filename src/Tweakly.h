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
 * TWEAKLY FRAMEWORK
 *
 *
 * Created by Mirko Pacioni
 * March 12th 2021
 *
 *
 * Comments added by G.Bruno (@gbr1) on May 28th 2021
 */

#ifndef TWEAKLY_H
#define TWEAKLY_H

#include <Arduino.h>

using namespace std;

//Definitions support
#include "definitions.h"

//Include machines
#if defined(ARDUINO_ARCH_ESP32)
  #include "core/machines/esp32.h"
  using namespace esp32;
#elif defined(ARDUINO_ARCH_ESP8266)
  #include "core/machines/esp8266.h"
  using namespace esp8266;
#else
  #include "core/machines/arduino_boards.h"
  using namespace arduino_boards;
#endif

//Include core libs
#include "core/pads.h"
#include "core/timers.h"
#include "core/polyphonic.h"
#include "core/clock.h"
#include "core/string.h"
#include "core/numbers.h"

using namespace tweaklypads;
using namespace tweaklyticktimers;
using namespace tweaklypolyphonic;
using namespace tweaklyclock;
using namespace tweaklystring;
using namespace tweaklynumbers;

//Include peripherals libs
#include "peripherals/sonar.h"
#include "peripherals/encoder.h"

using namespace sonar;
using namespace encoder;

//Include add-on
#include "user_integrations.h"

// Enablers
volatile bool _tweakly_ready { false };

// TweaklyRun: run the core; must be called in loop
void TweaklyRun(){
  if (!_tweakly_ready){
    //core
    tweaklypads::Setup();
    tweaklyticktimers::Setup();
    tweaklypolyphonic::Setup();
    tweaklyclock::Setup();
    //peripherals
    sonar::Setup();
    encoder::Setup();
    _tweakly_ready = true;
  } else {
    //core
    tweaklypads::Loop();
    tweaklyticktimers::Loop();
    tweaklypolyphonic::Loop();
    tweaklyclock::Loop();
    //peripherals
    sonar::Loop();
    encoder::Loop(); 
  }
}

#endif
