#include <Arduino.h> // for map
#include "rc_servo.hpp"

RCServo::RCServo (int pin) : _pin(pin) {}

uint8_t  RCServo::attach() const
{
  return _servo.attach(_pin);
}

void RCServo::write(int value) const
{
  _servo.write(map(value, 0, 1024, 0, 180));
}