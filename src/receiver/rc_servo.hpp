#include <stdint.h> //for uint8_t
#include <Servo.h>

class RCServo
{
  public:
    RCServo(int pin, long to_low, long to_high); // constructor
    uint8_t  attach() const;
    void write(int value) const; 

  private:
    const Servo _servo;
    const int _pin; // the pin the servo is connected on board 
    const long _to_low;
    const long _to_high;
};