/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 * RC RECEIVER
 *
 * 
 */

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> // required for radio.printDetails
#include "rc_servo.hpp"
#include "payload.hpp"
#include "secrets.hpp"

//#define DEBUG_FLAG

#ifdef DEBUG_FLAG
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)

//RF24_PA_MIN = 0, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX,
  const uint8_t RADIO_PA_LEVEL = RF24_PA_LOW;
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)

  const uint8_t RADIO_PA_LEVEL = RF24_PA_LOW;
#endif

RF24 radio(9, 8);  // nRF24L01(+) radio attached using Getting Started board

// Customize here pulse lengths as needed on the ESC docs
const int MIN_THROTTLE_PULSE_LENGTH = 1000; // Minimum pulse length in µs
const int MAX_THROTTLE_PULSE_LENGTH = 2000; // Maximum pulse length in µs

// Customize here the servo angle as needed on the servo docs
const int MIN_SERVO_ANGLE = 0; // Minimum servo angle
const int MAX_SERVO_ANGLE = 180; // Maximum servo angle

/**
* The servo controlling the plane
* On the throttle_servo we write pulse width microseconds 1000-2000.
* On the other servo we write an angle 0-180
*
*/
RCServo throttle_servo (3, MIN_THROTTLE_PULSE_LENGTH, MAX_THROTTLE_PULSE_LENGTH);
RCServo rudder_servo   (2, MIN_SERVO_ANGLE,           MAX_SERVO_ANGLE);
RCServo elevator_servo (4, MIN_SERVO_ANGLE,           MAX_SERVO_ANGLE);
RCServo aileron_servo  (5, MIN_SERVO_ANGLE,           MAX_SERVO_ANGLE);

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println("RF TX Begin Setup");

  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {
      delay(1000);
      // hold in infinite loop
    }
  }

  //radio.setDataRate(RF24_250KBPS);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RADIO_PA_LEVEL);
  radio.setChannel(SECRET_RADIO_CHANNEL);
  radio.setPayloadSize(PAYLOAD_SIZE); 
  radio.openReadingPipe(1, SECRET_PIPE_ADDRESS);  // Set the receiving pipe address
  radio.startListening();

  printf_begin(); // required for radio.printDetails
  Serial.println("\n\n >>>>>>>>>  printPrettyDetails <<<<<<<<");
  radio.printPrettyDetails(); 
  Serial.println("\n\n");
  Serial.println(" >>>>>>>>>  printDetails <<<<<<<<");
  radio.printDetails();

  // Attach servo and check if everything is ok
  if
  (
    throttle_servo.attach() == INVALID_SERVO ||
    rudder_servo.attach()   == INVALID_SERVO ||
    elevator_servo.attach() == INVALID_SERVO ||
    aileron_servo.attach()  == INVALID_SERVO
  ){
    while (1) {
      Serial.println("Servo not responding!");  // hold in infinite loop
      delay(1000);
    }
  }

  Serial.println("\n\nRF RX End Setup");
}

void loop(void) {
  while (radio.available()) {  // Is there anything ready for us? //TODO: na perasw to pipe ws parametro?
// If so, grab it and print it out
    payload_t payload;
    radio.read(&payload, PAYLOAD_SIZE);

    //Serial.print(F("rudder="));
    //Serial.println(payload.rudder);

    DEBUG_PRINT("throttle=");
    DEBUG_PRINTLN(payload.throttle);

    //DEBUG_PRINT("elevator=");
    //DEBUG_PRINTLN(payload.elevator);

    //DEBUG_PRINT("aileron=");
    //DEBUG_PRINTLN(payload.aileron);

    throttle_servo.writeMicroseconds(payload.throttle);
    rudder_servo.write(payload.rudder);
    elevator_servo.write(payload.elevator);
    aileron_servo.write(payload.aileron);
  }
}
