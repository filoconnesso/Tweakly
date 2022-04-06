<br />
<p align="center">
  <a href="https://github.com/filoconnesso/Tweakly/">
    <img src="https://www.filoconnesso.it/tweakly/assets/images/logo.png" alt="Logo">
  </a>
  <p align="center">
    Tweakly allows you to create dynamic code, using programming methods that are normally difficult to implement.
    <br />
    <a href="https://www.filoconnesso.it/tweakly"><strong>Visit the website »</strong></a>
    <br />
    <br />
    <a href="https://www.youtube.com/watch?v=nspS5Prc-5I">View Demo</a>
    ·
    <a href="https://github.com/filoconnesso/Tweakly/issues">Report Bug</a>
    ·
    <a href="https://github.com/filoconnesso/Tweakly/pulls">Create PR</a>
  </p>
</p>


[![filoconnesso - Tweakly](https://img.shields.io/static/v1?label=filoconnesso&message=Tweakly&color=blue&logo=github)](https://github.com/filoconnesso/Tweakly)
[![stars - Tweakly](https://img.shields.io/github/stars/filoconnesso/Tweakly?style=social)](https://github.com/filoconnesso/Tweakly)
[![forks - Tweakly](https://img.shields.io/github/forks/filoconnesso/Tweakly?style=social)](https://github.com/filoconnesso/Tweakly)

[![GitHub tag](https://img.shields.io/github/tag/filoconnesso/Tweakly?include_prereleases=&sort=semver)](https://github.com/filoconnesso/Tweakly/releases/)
[![License](https://img.shields.io/badge/License-GPL_3.0-blue)](#license)
[![issues - Tweakly](https://img.shields.io/github/issues/filoconnesso/Tweakly)](https://github.com/filoconnesso/Tweakly/issues)

[![ESP32 Compatible](https://img.shields.io/badge/Supported-yes-green.svg)](https://github.com/filoconnesso/Tweakly)
[![Arduino Lint Passed](https://img.shields.io/badge/Arduino%20Lint%20Passed-yes-green.svg)](https://github.com/filoconnesso/Tweakly)
[![Arduino Ide Library Manager](https://img.shields.io/badge/Arduino%20Ide%20Library%20Manager-yes-green.svg)](https://github.com/filoconnesso/Tweakly)
[![Arduino Boards Compatible](https://img.shields.io/badge/Arduino%20Boards%20Compatible-yes-green.svg)](https://github.com/filoconnesso/Tweakly)
[![ESP8266 Compatible](https://img.shields.io/badge/ESP8266%20Compatible-yes-green.svg)](https://github.com/filoconnesso/Tweakly)
[![ESP32 Compatible](https://img.shields.io/badge/ESP32%20Compatible-yes-green.svg)](https://github.com/filoconnesso/Tweakly)

## About The Project

**Tweakly** allows you to create dynamic code, **using programming methods that are normally difficult to implement**, allowing the user to reduce the writing of the code as much as possible to allow the latter to concentrate on the project to be implemented.

## Getting Started

To use Tweakly you only need the Arduino IDE and one of the boards that work with it.

In development testing Tweakly was tested on:

* Arduino Uno
* Attiny85
* ESP8266
* ESP32
* SAMD21 (Arduino MRK Wi-Fi 1010 and Arduino Nano 33 IoT)

### Documentation

The Tweakly documentation is under development, it will be available in the coming months in Italian and English, in the meantime make use of the included examples and wait patiently :-)

**For advanced examples you can visit the dedicated repository : https://github.com/filoconnesso/tweakly-examples**

### Installation

1. From Arduino CLI
   ```sh
   arduino-cli lib search Tweakly
   ```
   ```sh
   arduino-cli lib install Tweakly
   ```
2. From IDE
   Search "Tweakly" from **Sketch->#include library->Library Manager**

## Usage

Tweakly bases its operation on **millis()** and allows you to create non-blocking code for your board.

It is essential to know that to work correctly you need to use methods that do not block the execution of your code, the main libraries available for the sensors may have delays inside them, but after the tests done in the development phase we have could verify that most of the libraries are tweakly-compatible.

Tweakly takes pin status into account with a built-in pin manager, so use that to initialize your pins:

   ```cpp
   Pad led(13);
   ```
With Tweaky's pin manager you can read and write values ​​on all initialized pins and give them a name as you like, making your life a lot easier!

   ```cpp
   led.write(1);
   led.read(); //1
   ```
If you want to create non-blocking code, rely on TickTimers.

   ```cpp
   TickTimer blinker;
   ```
Initialize it in **setup()** with:
 
   ```cpp
   blinker.attach(1000, [] { led.toggle(); });
   ```
And your led will start blinking without blocking your code! Is not it fantastic ?

_For more features, please see the [Examples Folder](https://github.com/filoconnesso/Tweakly/tree/main/examples)_

## Tricks

Use a fast baud rate to not affect the performance of your code: 
   ```cpp
   Serial.begin(115200);
   ```
Use stringAssembler to assemble strings and format a dataset together: 
   ```cpp
   int sensor_value;
   char sensor_name;
   
   ...
   
   String message = stringAssembler("your sensor %s has read %d C° \n", sensor_name, sensor_value);
   ```
Use Echo Stream for print to file and Serial :
   ```cpp
   Echo printer;
   ```
   ```cpp
   Serial.begin(115200);
   
   printer.attach(&Serial);
   printer.print("Hello, Friend \n");
   ```
## Demo

[![Watch the video](https://img.youtube.com/vi/nspS5Prc-5I/hqdefault.jpg)](https://youtu.be/nspS5Prc-5I)

In this video the first green LED flashes with at 250ms timer, the red LED flashes at 1000ms and the blue LED at 1500ms. The last two red LEDs have a pwm control.

Touch button # 3 is the play button, Touchpad # 2 freezes the melody and button # 1 pauses it.

Tweakly's new "Clock" component was used for the clock, which allows you to create virtual clocks and manage them separately.

All the code is handled by various TickTimers that guarantee non-blocking execution of everything.

Code : https://create.arduino.cc/editor/filoconnesso/0347b6f1-775b-4f70-bb65-e5fa197c5b39/preview

## Let's create a community 

If you use Tweakly to create a project, tag us on Instagram and Twitter with the tag **@filoconnesso** and use the hashtag **#createdwithtweakly**. If you like the project don't forget to leave a star on GitHub and if you have tips and tricks to improve Tweakly create a Pull Request, we will try to accommodate you! 

## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Released under [GPL](/LICENSE) by [@filoconnesso](https://github.com/filoconnesso).

Distributed under the open source GPL v.3.0 license. Read the license [here](https://github.com/filoconnesso/Tweakly/blob/main/LICENSE).

In short :

Copyright (C) 2021  Filo Connesso - filoconnesso.it

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Contact

Filo Connesso - [@FiloConnesso](https://twitter.com/FiloConnesso) - redazione@filoconnesso.it

Mirko Pacioni - [@PacioniMirko](https://twitter.com/PacioniMirko) - mirko.pacioni@outlook.com

WebSite : [https://www.filoconnesso.it/tweakly/](https://www.filoconnesso.it/tweakly/)