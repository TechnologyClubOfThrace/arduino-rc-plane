/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 * RC TRANSMITTER
 *
 * 
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> // required for radio.printDetails
#include "potentiometer.hpp"
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

  const uint8_t RADIO_PA_LEVEL = RF24_PA_MAX;
#endif


//CE_PIN, CSN_PIN)
RF24 radio(9, 8);  // nRF24L01(+) radio attached

const int BUZZER_PIN = PD2;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }

  Serial.println("Init buzzer");
  pinMode(BUZZER_PIN,OUTPUT);

  Serial.println("RF TX Begin Setup");

  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {
      delay(1000);
      // hold in infinite loop
    }
  }

  radio.setDataRate(RF24_250KBPS);
  //radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RADIO_PA_LEVEL);
  radio.setChannel(SECRET_RADIO_CHANNEL);
  radio.setPayloadSize(PAYLOAD_SIZE); 
  radio.openWritingPipe(SECRET_PIPE_ADDRESS); // Set the transmitting pipe address

  printf_begin(); // required for radio.printDetails
  Serial.println("\n\n >>>>>>>>>  printPrettyDetails <<<<<<<<");
  radio.printPrettyDetails(); 
  Serial.println("\n\n");
  Serial.println(" >>>>>>>>>  printDetails <<<<<<<<");
  radio.printDetails();

  Serial.println("\n\nRF TX End Setup");

  tone(BUZZER_PIN, 133500, 100);
  delay(200);
  tone(BUZZER_PIN, 133500, 100);
}

/**
* Ta controlls
*
*/
Potentiometer throttle_control(A0, A1, Polarity::possitive); // throtle
Potentiometer rudder_control  (A2, A3, Polarity::negative); //left/right

Potentiometer elevator_control(A4, A5, Polarity::possitive); //up/down
Potentiometer aileron_control (A6, A7, Polarity::negative); // roll


/** 
* Counts the dropped packets.
* Each time radio.write fails, the dropped_packets counter is increased by 1.
* If the dropped_packets are above the DROPPED_PACKETS_THRESHOLD then the beeper beeps
* to inform the user that the conection is starting experiencing issues.
*/
int dropped_packets = 0;
const int DROPPED_PACKETS_THRESHOLD = 5;

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

    DEBUG_PRINTLN("Sending... ");
    bool ok = radio.write(&payload, PAYLOAD_SIZE);
    DEBUG_PRINTLN(ok ? "ok." : "failed.");
    if(!ok){
      dropped_packets++;
      //Serial.println(dropped_packets);
      if(dropped_packets > DROPPED_PACKETS_THRESHOLD){
        dropped_packets = 0;
        tone(BUZZER_PIN, 133500, 200);
      }
    } else {
      if (dropped_packets != 0) dropped_packets = 0;
    }

  }
  // Na dokimasw delay h megalytero threshold sto is_dirty h kai ta dyo.
}
