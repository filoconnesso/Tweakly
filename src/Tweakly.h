/*
*
* LIBRARY FOR WIRING EXSPANSION
* Created by Mirko Pacioni
* 12/03/2021
*
*/

#include "Arduino.h"


#define OUT 1
#define IN 2
#define ALWAYS 3

typedef void (*_tick_callback)();
typedef void (*_encoder_callback)(bool);

bool _tweakly_ready = false;
bool _ticks_exists = false;
bool _pad_exists = false;
bool _encoder_exists = false;
bool _pwm_pad_exists = false;

unsigned long _pin_button_default_debounce_millis = 50;
unsigned long _pin_encoder_default_debounce_millis = 1;
unsigned long _pin_pwm_default_delay_millis = 0;

struct _ticks
{
    const char *_tick_name;
    unsigned long _tick_current_millis;
    unsigned long _tick_delay;
    unsigned long _tick_previous_time;
    bool _tick_enabled;
    _tick_callback _tick_callback_function;
    _ticks *_next_tick = NULL;
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
    const char *_pin_class;
    unsigned long _pin_debounce_current_millis;
    unsigned long _pin_debounce_previous_millis;
    unsigned long _pin_debounce_delay_millis;
    _pins *_next_pin = NULL;
};

struct _pwm_pins
{
    int _pwm_pin_number;
    unsigned int _pwm_pin_value;
    unsigned long _pwm_pin_delay_current_millis;
    unsigned long _pwm_pin_delay_previous_millis;
    bool _pwm_pin_fade_direction;
    unsigned long _pwm_max_value;
    unsigned long _pwm_min_value;
    char *_pwm_pin_class;
    bool _pwm_pin_enabled;
    _pwm_pins *_next_pwm_pin = NULL;
};

struct _encoders
{
    const char *_encoder_name;
    int _encoder_dt_pin;
    int _encoder_clk_pin;
    bool _encoder_dt_pin_status;
    bool _encoder_clk_pin_status;
    bool _encoder_clk_pin_previous_status;
    unsigned long _encoder_debounce_current_millis;
    unsigned long _encoder_debounce_previous_millis;
    unsigned long _encoder_debounce_delay_millis;
    _encoder_callback _encoder_change_callback;
    _encoders *_next_encoder = NULL;
};

_ticks *_first_tick = NULL, *_last_tick = NULL;
_pins *_first_pin = NULL, *_last_pin = NULL;
_encoders *_first_encoder = NULL, *_last_encoder = NULL;
_pwm_pins *_first_pwm_pin = NULL, *_last_pwm_pin = NULL;

void encoderAttach(int _new_encoder_dt_pin, int _new_encoder_clk_pin, _encoder_callback _new_encoder_change_callback)
{
    _encoders *_new_encoder = new _encoders;
    _new_encoder->_encoder_dt_pin = _new_encoder_dt_pin;
    _new_encoder->_encoder_clk_pin = _new_encoder_clk_pin;
    if (_first_encoder == NULL)
    {
        _first_encoder = _new_encoder;
    }
    else
    {
        _last_encoder->_next_encoder = _new_encoder;
    }
    _new_encoder->_encoder_change_callback = _new_encoder_change_callback;
    _new_encoder->_encoder_debounce_delay_millis = _pin_encoder_default_debounce_millis;
    _last_encoder = _new_encoder;
    if (!_encoder_exists)
    {
        _encoder_exists = true;
    }
}

void padMode(uint8_t _new_pin_number, uint8_t _new_pin_mode, uint8_t _new_pin_status, const char *_pin_class = "nope")
{
    _pins *_new_pin = new _pins;
    _pin_class = _pin_class;
    _new_pin->_pin_number = _new_pin_number;
    _new_pin->_pin_status = _new_pin_status;
    _new_pin->_pin_mode = _new_pin_mode;
    if (_first_pin == NULL)
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

void analogPadMode(uint8_t _new_pwm_pin_number, uint8_t _new_pwm_pin_start_value, uint8_t _new_pwm_pin_min, uint8_t _new_pwm_pin_max, char *_pin_class = "nope")
{
    _pwm_pins *_new_pwm_pin = new _pwm_pins;
    _new_pwm_pin->_pwm_pin_number = _new_pwm_pin_number;
    if (_first_pwm_pin == NULL)
    {
        _first_pwm_pin = _new_pwm_pin;
    }
    else
    {
        _last_pwm_pin->_next_pwm_pin = _new_pwm_pin;
    }
    pinMode(_new_pwm_pin_number, OUTPUT);
    _last_pwm_pin = _new_pwm_pin;
    _new_pwm_pin->_pwm_pin_class = _pin_class;
    _new_pwm_pin->_pwm_pin_value = _new_pwm_pin_start_value;
    _new_pwm_pin->_pwm_min_value = _new_pwm_pin_min;
    _new_pwm_pin->_pwm_max_value = _new_pwm_pin_max;
    _new_pwm_pin->_pwm_pin_fade_direction = false;
    _new_pwm_pin->_pwm_pin_enabled = true;
    if (!_pwm_pad_exists)
    {
        _pwm_pad_exists = true;
    }
}

void digitalToggle(int _digital_pin)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
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
        for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
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
        for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_status != _digital_status)
            {
                _this_pin->_pin_status = _digital_status;
                digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
            }
        }
    }
}

void digitalWriteClass(const char *_digital_pin_class, int _digital_status)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
        {
            if (_this_pin->_pin_mode == OUTPUT && _this_pin->_pin_status != _digital_status && _this_pin->_pin_class == _digital_pin_class && strcmp(_this_pin->_pin_class, "nope"))
            {
                _this_pin->_pin_status = _digital_status;
                digitalWrite(_this_pin->_pin_number, _this_pin->_pin_status);
            }
        }
    }
}

void analogWriteAll(int _analog_status)
{
    if (_pwm_pad_exists)
    {
        for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin)
        {
            if (_this_pwm_pin->_pwm_pin_enabled && _this_pwm_pin->_pwm_pin_value != _analog_status)
            {
                _this_pwm_pin->_pwm_pin_value = _analog_status;
                analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
            }
        }
    }
}

void analogEnable(int _pwm_pin_number, unsigned int _pwm_pin_value = 0)
{
    if (_pwm_pad_exists)
    {
        for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin)
        {
            if (!_this_pwm_pin->_pwm_pin_enabled)
            {
                _this_pwm_pin->_pwm_pin_value = _pwm_pin_value;
                _this_pwm_pin->_pwm_pin_enabled = true;
            }
        }
    }
}

void analogDisable(int _pwm_pin_number)
{
    if (_pwm_pad_exists)
    {
        for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin)
        {
            if (_this_pwm_pin->_pwm_pin_enabled)
            {
                _this_pwm_pin->_pwm_pin_enabled = false;
            }
        }
    }
}

void analogWriteClass(char *_pwm_pin_class, int _analog_status)
{
    if (_pwm_pad_exists)
    {
        for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin)
        {
            if (_this_pwm_pin->_pwm_pin_enabled && _this_pwm_pin->_pwm_pin_value != _analog_status && _this_pwm_pin->_pwm_pin_class == _pwm_pin_class && _this_pwm_pin->_pwm_pin_class != "nope")
            {
                _this_pwm_pin->_pwm_pin_value = _analog_status;
                analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
            }
        }
    }
}

void analogWriteFade(int _pwn_pin_number, unsigned long _delay, int _pwm_fade_mode)
{
    if (_pwm_pad_exists)
    {
        for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin)
        {
            if (_this_pwm_pin->_pwm_pin_enabled == true && _this_pwm_pin->_pwm_pin_number == _pwn_pin_number && _delay >= 0)
            {
                unsigned long _current_millis = millis();
                _this_pwm_pin->_pwm_pin_delay_current_millis = _current_millis;
                if (_this_pwm_pin->_pwm_pin_delay_current_millis - _this_pwm_pin->_pwm_pin_delay_previous_millis > _delay)
                {
                    switch (_pwm_fade_mode)
                    {
                    case ALWAYS:
                        if (!_this_pwm_pin->_pwm_pin_fade_direction)
                        {
                            _this_pwm_pin->_pwm_pin_value++;
                            if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_max_value)
                            {
                                _this_pwm_pin->_pwm_pin_fade_direction = true;
                            }
                        }
                        if (_this_pwm_pin->_pwm_pin_fade_direction)
                        {
                            _this_pwm_pin->_pwm_pin_value--;
                            if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_min_value)
                            {
                                _this_pwm_pin->_pwm_pin_fade_direction = false;
                            }
                        }
                        break;
                    case IN:
                        _this_pwm_pin->_pwm_pin_value++;
                        if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_max_value)
                        {
                            _this_pwm_pin->_pwm_pin_enabled = false;
                        }
                        break;
                    case OUT:
                        _this_pwm_pin->_pwm_pin_value--;
                        if (_this_pwm_pin->_pwm_pin_value == _this_pwm_pin->_pwm_min_value)
                        {
                            _this_pwm_pin->_pwm_pin_enabled = false;
                        }
                        break;
                    }
                    analogWrite(_this_pwm_pin->_pwm_pin_number, _this_pwm_pin->_pwm_pin_value);
                    _this_pwm_pin->_pwm_pin_delay_previous_millis = _this_pwm_pin->_pwm_pin_delay_current_millis;
                }
            }
        }
    }
}

bool digitalPushButton(int _digital_pin)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
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
    return 0;
}

bool digitalSwitchButton(int _digital_pin)
{
    if (_pad_exists)
    {
        for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
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
    return 0;
}

void setTick(const char *_new_tick_name, unsigned long _new_tick_delay, _tick_callback _new_tick_callback)
{
    _ticks *_new_tick = new _ticks;
    _new_tick->_tick_name = _new_tick_name;
    _new_tick->_tick_delay = _new_tick_delay;
    _new_tick->_tick_callback_function = _new_tick_callback;
    if (_first_tick == NULL)
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

void pauseTick(const char *_tick_name)
{
    if (_ticks_exists)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick)
        {
            if (_this_tick->_tick_name == _tick_name)
            {
                _this_tick->_tick_enabled = 0;
            }
        }
    }
}

void playTick(const char *_tick_name)
{
    if (_ticks_exists)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick)
        {
            if (_this_tick->_tick_name == _tick_name)
            {
                _this_tick->_tick_enabled = 1;
            }
        }
    }
}

bool tickIsRunning(const char* _tick_name)
{
    _tick_name = _tick_name;
    if (_ticks_exists)
    {
        for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick)
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
    return 0;
}

void TweaklyRun()
{
    unsigned long _current_millis = millis();
    if (!_tweakly_ready)
    {
        if (_ticks_exists)
        {
            for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick)
            {
                _this_tick->_tick_previous_time = _current_millis;
            }
        }
        if (_pad_exists)
        {
            for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
            {
                if (_this_pin->_pin_mode != OUTPUT)
                {
                    _this_pin->_pin_debounce_previous_millis = _current_millis;
                }
            }
        }
        if (_pwm_pad_exists)
        {
            for (_pwm_pins *_this_pwm_pin = _first_pwm_pin; _this_pwm_pin != NULL; _this_pwm_pin = _this_pwm_pin->_next_pwm_pin)
            {
                if (_this_pwm_pin->_pwm_pin_enabled)
                {
                    _this_pwm_pin->_pwm_pin_delay_current_millis = _current_millis;
                }
            }
        }
        if (_encoder_exists)
        {
            for (_encoders *_this_encoder = _first_encoder; _this_encoder != NULL; _this_encoder = _this_encoder->_next_encoder)
            {

                _this_encoder->_encoder_debounce_previous_millis = _current_millis;
                _this_encoder->_encoder_clk_pin_previous_status = digitalRead(_this_encoder->_encoder_clk_pin);
            }
        }
        _tweakly_ready = true;
    }
    else
    {
        if (_ticks_exists)
        {
            for (_ticks *_this_tick = _first_tick; _this_tick != NULL; _this_tick = _this_tick->_next_tick)
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
            for (_pins *_this_pin = _first_pin; _this_pin != NULL; _this_pin = _this_pin->_next_pin)
            {
                _this_pin->_pin_debounce_current_millis = _current_millis;
                if (_this_pin->_pin_mode != OUTPUT)
                {
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
    if (_encoder_exists)
    {
        for (_encoders *_this_encoder = _first_encoder; _this_encoder != NULL; _this_encoder = _this_encoder->_next_encoder) {
            _this_encoder->_encoder_debounce_current_millis = _current_millis;
            if (_this_encoder->_encoder_debounce_current_millis - _this_encoder->_encoder_debounce_previous_millis > _this_encoder->_encoder_debounce_delay_millis)
            {
                _this_encoder->_encoder_clk_pin_status = digitalRead(_this_encoder->_encoder_clk_pin);
                _this_encoder->_encoder_debounce_previous_millis = _this_encoder->_encoder_debounce_current_millis;
                if (_this_encoder->_encoder_clk_pin_status != _this_encoder->_encoder_clk_pin_previous_status && _this_encoder->_encoder_clk_pin_status == HIGH)
                {
                    _this_encoder->_encoder_dt_pin_status = digitalRead(_this_encoder->_encoder_dt_pin);
                    _this_encoder->_encoder_change_callback(_this_encoder->_encoder_dt_pin_status);
                }
                _this_encoder->_encoder_clk_pin_previous_status = _this_encoder->_encoder_clk_pin_status;
            }
        }
    }
}
