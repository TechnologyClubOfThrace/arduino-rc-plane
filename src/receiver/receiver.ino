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
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(9, 8);  // nRF24L01(+) radio attached using Getting Started board

Servo servo;
Servo rudder_servo;

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
  radio.setPayloadSize(sizeof(struct payload_t)); 
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);  // Set the receiving pipe address
  radio.startListening();

  //Include the servo motor pin
  servo.attach(3);
  rudder_servo.attach(2);
}

void loop(void) {
  while (radio.available()) {  // Is there anything ready for us?
// If so, grab it and print it out
    payload_t payload;
    radio.read(&payload, sizeof(payload));
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
  }
}
