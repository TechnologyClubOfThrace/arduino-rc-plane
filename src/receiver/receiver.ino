/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 *
 *
 * 
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>


RF24 radio(9, 8);  // nRF24L01(+) radio attached using Getting Started board

Servo servo;
Servo rudder_servo;

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 00;   // Address of our node in Octal format (04, 031, etc)
const uint16_t other_node = 01;  // Address of the other node in Octal format

struct payload_t {  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
  unsigned int elevator;
  unsigned int rudder;
};


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
  radio.setChannel(87);
  network.begin(/*node address*/ this_node);

  //Include the servo motor pin
  servo.attach(3);
  rudder_servo.attach(2);
}

void loop(void) {

  network.update();  // Check the network regularly

  while (network.available()) {  // Is there anything ready for us?

    RF24NetworkHeader header;  // If so, grab it and print it out
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    Serial.print(F("Received packet: counter="));
    Serial.print(payload.counter);
    Serial.print(F(", elevator="));
    Serial.print(payload.elevator);
    Serial.print(F(", rudder="));
    Serial.print(payload.rudder);
    Serial.print(F(", origin timestamp="));
    Serial.println(payload.ms);

    //Write these values on the servo
    servo.write(payload.elevator);
    rudder_servo.write(payload.rudder);
  }
}
