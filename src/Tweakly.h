/*
*
* LIBRARY FOR WIRING ESPANSION
* Created by Mirko Pacioni
* 12/03/2021
*
*/

#include "Arduino.h"

bool _tweakly_ready = false;
bool _ticks_exists = false;
bool _pad_exists = false;

typedef void (*_tick_callback)();

unsigned long _pin_button_default_debounce_millis = 50;

struct _ticks
{
    char _tick_name;
    unsigned long _tick_current_millis;
    unsigned long _tick_delay;
    unsigned long _tick_previous_time;
    bool _tick_enabled;
    _tick_callback _tick_callback_function;
    _ticks *_next_tick = nullptr;
};

struct _pins
{
    int _pin_number;
    bool _pin_status;
    bool _pin_previous_status;
    bool _pin_debounced_status;
    bool _pin_debounced_start_status;
    bool _pin_switch_status;
    bool _pin_switch_release_button;
    int _pin_mode;
    char _pin_class;
    unsigned long _pin_debounce_current_millis;
    unsigned long _pin_debounce_previous_millis;
    unsigned long _pin_debounce_delay_millis;
    _pins *_next_pin = nullptr;
};

_ticks *_first_tick = nullptr, *_last_tick = nullptr;
_pins *_first_pin = nullptr, *_last_pin = nullptr;

void padMode(uint8_t _new_pin_number, uint8_t _new_pin_mode, uint8_t _new_pin_status, char _pin_class = "nope")
{
    _pins *_new_pin = new _pins;
    _new_pin->_pin_number = _new_pin_number;
    _new_pin->_pin_status = _new_pin_status;
    _new_pin->_pin_mode = _new_pin_mode;
    if (_first_pin == nullptr)
    {
        _first_pin = _new_pin;
    }
    else
    {
        _last_pin->_next_pin = _new_pin;
    }
    if (_new_pin_mode != OUTPUT)
    {
        _new_pin->_pin_previous_status = _new_pin_status;
        _new_pin->_pin_debounce_delay_millis = _pin_button_default_debounce_millis;
        _new_pin->_pin_debounced_status = 0;
        _new_pin->_pin_switch_status = _new_pin_status;
        _new_pin->_pin_switch_release_button = 1;
    }
    pinMode(_new_pin_number, _new_pin_mode);
    if (_new_pin_mode == OUTPUT)
    {
        digitalWrite(_new_pin_number, _new_pin_status);
    }
    _last_pin = _new_pin;
    if (!_pad_exists)
    {
        _pad_exists = true;
    }
}

void digitalToggle(int _digital_pin)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_number == _digital_pin && _this_pin->_pin_mode == OUTPUT)
            {
                _this_pin->_pin_status = !_this_pin->_pin_status;
                digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
            }
        }
    }
}

void digitalToggleAll()
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode == OUTPUT)
            {
                _this_pin->_pin_status = !_this_pin->_pin_status;
                digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
            }
        }
    }
}

void digitalWriteAll(int _digital_status)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_status != _digital_status)
            {
                _this_pin->_pin_status = _digital_status;
                digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
            }
        }
    }
}

void digitalWriteClass(char _digital_pin_class, int _digital_status)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_status != _digital_status && _this_pin->_pin_class == _digital_pin_class && _this_pin->_pin_class != "nope")
            {
                _this_pin->_pin_status = _digital_status;
                digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
            }
        }
    }
}

bool digitalPushButton(int _digital_pin)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode != OUTPUT)
            {
                if (_this_pin->_pin_number == _digital_pin)
                {
                    return !_this_pin->_pin_switch_release_button;
                }
            }
        }
    }
}

bool digitalSwitchButton(int _digital_pin)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode != OUTPUT)
            {
                if (_this_pin->_pin_number == _digital_pin && _this_pin->_pin_switch_release_button == 1)
                {
                    return _this_pin->_pin_switch_status;
                }
            }
        }
    }
}

void setTick(char _new_tick_name, unsigned long _new_tick_delay, _tick_callback _new_tick_callback)
{
    _ticks *_new_tick = new _ticks;
    _new_tick->_tick_name = _new_tick_name;
    _new_tick->_tick_delay = _new_tick_delay;
    _new_tick->_tick_callback_function = _new_tick_callback;
    if (_first_tick == nullptr)
    {
        _first_tick = _new_tick;
    }
    else
    {
        _last_tick->_next_tick = _new_tick;
    }
    _new_tick->_tick_enabled = 1;
    _new_tick->_tick_previous_time = 0;
    _last_tick = _new_tick;
    if (!_ticks_exists)
    {
        _ticks_exists = true;
    }
}

void pauseTick(char _tick_name)
{
    if (_ticks_exists)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != nullptr; _this_tick = _this_tick->_next_tick)
        {
            if (_this_tick->_tick_name == _tick_name)
            {
                _this_tick->_tick_enabled = 0;
            }
        }
    }
}

void playTick(char _tick_name)
{
    if (_ticks_exists)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != nullptr; _this_tick = _this_tick->_next_tick)
        {
            if (_this_tick->_tick_name == _tick_name)
            {
                _this_tick->_tick_enabled = 1;
            }
        }
    }
}

bool tickIsRunning(char _tick_name)
{
    if (_ticks_exists)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != nullptr; _this_tick = _this_tick->_next_tick)
        {
            if (!_this_tick->_tick_enabled)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
}

void TweaklyRun()
{
    unsigned long _current_millis = millis();
    if (!_tweakly_ready)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != nullptr; _this_tick = _this_tick->_next_tick)
        {
            _this_tick->_tick_previous_time = _current_millis;
        }
        for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode != OUTPUT)
            {
                _this_pin->_pin_debounce_previous_millis = _current_millis;
            }
        }
        _tweakly_ready = true;
    }
    else
    {
        if (_ticks_exists)
        {
            for (_ticks *_this_tick = _first_tick; _this_tick != nullptr; _this_tick = _this_tick->_next_tick)
            {
                if (_this_tick->_tick_enabled)
                {
                    _this_tick->_tick_current_millis = _current_millis;
                    if (_this_tick->_tick_current_millis - _this_tick->_tick_previous_time > _this_tick->_tick_delay)
                    {
                        _this_tick->_tick_previous_time = _this_tick->_tick_current_millis;
                        _this_tick->_tick_callback_function();
                    }
                }
            }
        }
        if (_pad_exists)
        {
            for (_pins *_this_pin = _first_pin; _this_pin != nullptr; _this_pin = _this_pin->_next_pin)
            {
                if (_this_pin->_pin_mode != OUTPUT)
                {
                    _this_pin->_pin_debounce_current_millis = _current_millis;
                    _this_pin->_pin_status = digitalRead(_this_pin->_pin_number);
                    if (_this_pin->_pin_status != _this_pin->_pin_previous_status)
                    {
                        _this_pin->_pin_debounce_previous_millis = _this_pin->_pin_debounce_current_millis;
                        _this_pin->_pin_switch_release_button = 1;
                    }
                    if (_this_pin->_pin_debounce_current_millis - _this_pin->_pin_debounce_previous_millis > _this_pin->_pin_debounce_delay_millis &&
                        _this_pin->_pin_status == _this_pin->_pin_previous_status)
                    {
                        _this_pin->_pin_debounce_previous_millis = _this_pin->_pin_debounce_current_millis;
                        _this_pin->_pin_debounced_status = _this_pin->_pin_status;
                        if (_this_pin->_pin_switch_release_button == 1)
                        {
                            _this_pin->_pin_switch_release_button = 0;
                            _this_pin->_pin_switch_status = !_this_pin->_pin_switch_status;
                        }
                    }
                }
            }
        }
    }
}