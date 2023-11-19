/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 * RC TRANSMITTER
 *
 * 
 */

 //TODO: -use DEBUG_PRINT, -abs, 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//#define DEBUG_FLAG

#ifdef DEBUG_FLAG
  #define DEBUG_PRINT   (x)  Serial.print   (x)
  #define DEBUG_PRINTLN (x)  Serial.println (x)

//RF24_PA_MIN = 0, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX,
  const uint8_t RADIO_PA_LEVEL = RF24_PA_LOW;
#else
  #define DEBUG_PRINT   (x)
  #define DEBUG_PRINTLN (x)

  const uint8_t RADIO_PA_LEVEL = RF24_PA_MAX;
#endif

#include "potentiometer.hpp"
#include "payload.hpp"
#include "secrets.hpp"


//CE_PIN, CSN_PIN)
RF24 radio(9, 8);  // nRF24L01(+) radio attached

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF TX Begin Setup"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      delay(1000);
      Serial.println(F("Radio hardware not responding!"));
      // hold in infinite loop
    }
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RADIO_PA_LEVEL);
  radio.setChannel(SECRET_RADIO_CHANNEL);
  radio.setPayloadSize(PAYLOAD_SIZE); 
  radio.openWritingPipe(SECRET_PIPE_ADDRESS); // Set the transmitting pipe address
}

/**
* Ta controlls
*
*/
Potentiometer throttle_control(A0, Potentiometer::TRIM_DISABLED); // throtle
Potentiometer rudder_control  (A2, Potentiometer::TRIM_DISABLED); //left/right

Potentiometer elevator_control(A4, Potentiometer::TRIM_DISABLED); //up/down
Potentiometer aileron_control (A6, Potentiometer::TRIM_DISABLED); // roll


void loop() {
  throttle_control.read();
  rudder_control.read();
  elevator_control.read();
  aileron_control.read();

  if (throttle_control.is_dirty() || rudder_control.is_dirty() || elevator_control.is_dirty() || aileron_control.is_dirty())
  {
    payload_t payload;
    payload = (payload_t) {
      .throttle = throttle_control.get_value(),
      .rudder   = rudder_control.get_value(),
      .elevator = elevator_control.get_value(),
      .aileron  = aileron_control.get_value(),
    };

    Serial.print(F("Sending... "));
    bool ok = radio.write(&payload, PAYLOAD_SIZE);
    Serial.println(ok ? F("ok.") : F("failed."));
  }
}
