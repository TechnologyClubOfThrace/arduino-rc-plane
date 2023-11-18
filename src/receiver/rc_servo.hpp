#include <stdint.h> //for uint8_t
#include <Servo.h>

class RCServo
{
  public:
    RCServo(int pin); // constructor
    uint8_t  attach() const;
    void write(int value) const; 

  private:
    Servo _servo;
    int _pin; // the pin the servo is connected on board 
};