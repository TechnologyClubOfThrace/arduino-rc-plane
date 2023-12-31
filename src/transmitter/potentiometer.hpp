/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 *  class Potentiometer
 *  
 * 
 */

#include <stdint.h>
#include "polarity.hpp"

class Potentiometer
{
  public:
    Potentiometer(uint8_t value_pin, uint8_t trim_pin, Polarity trim_polarity); // constructor
    void read();
    int  get_value() const;
    bool is_dirty()  const;
    static const uint8_t TRIM_DISABLED;
  
  private:
    const uint8_t _value_pin; // the pin the potentiometer is connected to on board
    const uint8_t _trim_pin; // the pin the potentiometer is connected to on board
    const Polarity _trim_polarity; // the pin the potentiometer is connected to on board
    int     _value;
    int     _last_value;
    bool    _is_dirty;
};