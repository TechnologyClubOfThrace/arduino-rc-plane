#include <Arduino.h> // for map
#include "rc_servo.hpp"

RCServo::RCServo (int pin, long to_low, long to_high) : _pin(pin), _to_low(to_low), _to_high(to_high) {}

uint8_t  RCServo::attach() const
{
  return _servo.attach(_pin);
}

void RCServo::write(int value) const
{
  _servo.write(map(value, 0, 1024, _to_low, _to_high));
}