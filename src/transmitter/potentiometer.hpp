/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 *  class Potentiometer
 *  
 * 
 */

#include <stdint.h>
#include <limits.h>

class Potentiometer
{
  public:
    Potentiometer(uint8_t value_pin, uint8_t trim_pin); // constructor
    void read();
    int  get_value() const;
    bool is_dirty()  const;
    //static const uint8_t TRIM_DISABLED = numeric_limits<uint8_t>::max();
  
  private:
    uint8_t _value_pin; // the pin the potentiometer is connected to on board
    uint8_t _trim_pin; // the pin the potentiometer is connected to on board
    int     _value;
    int     _last_value;
    bool    _is_dirty;
};