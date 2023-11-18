/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 *
 *
 * 
 */

#include "potentiometer.hpp"
#include "payload.hpp"
#include "secrets.hpp"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//CE_PIN, CSN_PIN)
RF24 radio(9, 8);  // nRF24L01(+) radio attached

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF TX Begin Setup"));

  //radio.setPALevel(RF24_PA_LOW);

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      delay(1000);
      Serial.println(F("Radio hardware not responding!"));
      // hold in infinite loop
    }
  }
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(87);
  radio.setPayloadSize(PAYLOAD_SIZE); 
  radio.openWritingPipe(PIPE_ADDRESS); // Set the transmitting pipe address
}

int last_map_value = -1;
int last_rudder_value = -1;

/**
* Ta controlls
*
*/
Potentiometer throttle_control(A0, A1); // throtle
Potentiometer rudder_control(A2, A3); //left/right

Potentiometer elevator_control(A4, A5); //up/down
Potentiometer aileron_control(A6, A7); // roll


void loop() {
  throttle_control.read();
  rudder_control.read();
  elevator_control.read();
  aileron_control.read();

  if (throttle_control.is_dirty() || rudder_control.is_dirty() || elevator_control.is_dirty() || aileron_control.is_dirty())
  {
    payload_t payload;
    payload = (payload_t) {
      .ms = millis(),
      .counter = 1,
      .elevator = elevator_control.get_value(),
      .rudder   = rudder_control.get_value()
    };
  }
  
  int value = analogRead(A0);
  int rudder_value = analogRead(A1);

  int map_value = map(value, 0, 1024, 0, 180);
  int map_rudder_value = map(rudder_value, 0, 1024, 0, 180);

  if (last_map_value != map_value || last_rudder_value != map_rudder_value){
    last_map_value = map_value;
    last_rudder_value = map_rudder_value;

    //Convert these values from 0 to 180
    //Print these values on the serial monitor
    Serial.println(map_value);  // Check the network regularly

    Serial.print(F("Sending... "));
    payload_t payload = { 1, 1, map_value, map_rudder_value };
    bool ok = radio.write(&payload, PAYLOAD_SIZE);
    Serial.println(ok ? F("ok.") : F("failed."));
  }
}
