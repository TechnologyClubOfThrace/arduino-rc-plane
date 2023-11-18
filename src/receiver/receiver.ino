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

#include "rc_servo.hpp"

#include "payload.hpp"
#include "secrets.hpp"

RF24 radio(9, 8);  // nRF24L01(+) radio attached using Getting Started board

Servo servo;
Servo rudder_servo;

/**
* The servo controlling the plane
*
*/
RCServo throttle_servo(A0);
RCServo rudder_servo2(A1);
RCServo elevator_servo(A2);
RCServo aileron_servo(A3);

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_rx/"));

  //radio.setPALevel(RF24_PA_LOW);

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(87);
  radio.setPayloadSize(PAYLOAD_SIZE); 
  radio.openReadingPipe(1, PIPE_ADDRESS);  // Set the receiving pipe address
  radio.startListening();

  //Include the servo motor pin
  servo.attach(3);
  rudder_servo.attach(2);

  // Attach servo and check if everything is ok
  /*
  if
  (
    throttle_servo.attach() == INVALID_SERVO ||
    rudder_servo2.attach()  == INVALID_SERVO ||
    elevator_servo.attach() == INVALID_SERVO ||
    aileron_servo.attach()  == INVALID_SERVO
  ){
    while (1) {
      Serial.println(F("Servo not responding!"));  // hold in infinite loop
      delay(1000);
    }
  }
  */

}

void loop(void) {
  while (radio.available()) {  // Is there anything ready for us?
// If so, grab it and print it out
    payload_t payload;
    radio.read(&payload, PAYLOAD_SIZE);
    Serial.print(F("Received packet: counter="));
    Serial.print(payload.counter);
    Serial.print(F(", elevator 000="));
    Serial.print(payload.elevator);
    Serial.print(F(", rudder="));
    Serial.print(payload.rudder);
    Serial.print(F(", origin timestamp="));
    Serial.println(payload.ms);

    //Write these values on the servo
    servo.write(payload.elevator);
    rudder_servo.write(payload.rudder);

    //throttle_servo.write(123);


  }
}
