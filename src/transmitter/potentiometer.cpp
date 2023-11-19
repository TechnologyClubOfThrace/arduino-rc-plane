/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 *
 *
 * 
 */
 
#include "potentiometer.hpp"

#include <Arduino.h> // for analogRead

const uint8_t Potentiometer::TRIM_DISABLED = 255;

Potentiometer::Potentiometer (uint8_t value_pin, uint8_t trim_pin) : _value_pin(value_pin), _trim_pin(trim_pin) {}

void Potentiometer::read()
{
  long mapped_trim_value = _trim_pin != Potentiometer::TRIM_DISABLED ? map(analogRead(_trim_pin), 0, 1023, -300, 300) : 0;

  _value = constrain(analogRead(_value_pin) + mapped_trim_value, 0, 1023);

//TODO: maybe implement it like this instead of multiple analogReads: _is_dirty = abs(_last_value - _value) > 1;
  _is_dirty = (_last_value != _value);

  if (_is_dirty){
    _last_value = _value;
  }
}

int Potentiometer::get_value() const
{
  return _value;
}

bool Potentiometer::is_dirty() const
{
  return _is_dirty;
}