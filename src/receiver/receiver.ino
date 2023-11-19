/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 * RC RECEIVER
 *
 * 
 */

//TODO: -use DEBUG_PRINT

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "rc_servo.hpp"

#include "payload.hpp"
#include "secrets.hpp"

const uint8_t RADIO_PA_LEVEL = RF24_PA_MAX;

RF24 radio(9, 8);  // nRF24L01(+) radio attached using Getting Started board

/**
* The servo controlling the plane
*
*/
RCServo throttle_servo (3, 0, 120);
RCServo rudder_servo   (2, 0, 180);
RCServo elevator_servo (4, 0, 180);
RCServo aileron_servo  (5, 0 ,180);

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_rx/"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RADIO_PA_LEVEL);
  radio.setChannel(SECRET_RADIO_CHANNEL);
  radio.setPayloadSize(PAYLOAD_SIZE); 
  radio.openReadingPipe(1, SECRET_PIPE_ADDRESS);  // Set the receiving pipe address
  radio.startListening();

  // Attach servo and check if everything is ok
  if
  (
    throttle_servo.attach() == INVALID_SERVO ||
    rudder_servo.attach()   == INVALID_SERVO ||
    elevator_servo.attach() == INVALID_SERVO ||
    aileron_servo.attach()  == INVALID_SERVO
  ){
    while (1) {
      Serial.println(F("Servo not responding!"));  // hold in infinite loop
      delay(1000);
    }
  }
}

void loop(void) {
  while (radio.available()) {  // Is there anything ready for us?
// If so, grab it and print it out
    payload_t payload;
    radio.read(&payload, PAYLOAD_SIZE);

    //Serial.print(F("rudder="));
    //Serial.println(payload.rudder);

    Serial.print(F("throttle="));
    Serial.println(payload.throttle);

    //Serial.print(F("elevator="));
    //Serial.print(payload.elevator);

    //Serial.print(F("aileron="));
    //Serial.println(payload.aileron);

    throttle_servo.write(payload.throttle);
    rudder_servo.write(payload.rudder);
    elevator_servo.write(payload.elevator);
    aileron_servo.write(payload.aileron);

  }
}
