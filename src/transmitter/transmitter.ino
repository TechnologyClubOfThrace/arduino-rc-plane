/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 *
 *
 * 
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//CE_PIN, CSN_PIN)
RF24 radio(9, 8);  // nRF24L01(+) radio attached

unsigned long last_sent;     // When did we last send?
unsigned long packets_sent;  // How many have we sent already


struct payload_t {  // Structure of our payload
  unsigned long  ms;
  unsigned long counter;
  unsigned int  elevator;
  unsigned int  rudder;
  // unsigned float BatteryVoltage;
  // unsigned float x,y,z;
};

uint8_t payload_size = sizeof(struct payload_t);

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
  radio.setPayloadSize(payload_size); 
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Set the transmitting pipe address
}

int last_map_value = -1;
int last_rudder_value = -1;

void loop() {

  unsigned long now = millis();
  
  int value = analogRead(A0);
  int rudder_value = analogRead(A1);

  int map_value = map(value, 0, 1024, 0, 180);
  int map_rudder_value = map(rudder_value, 0, 1024, 0, 180);

  if (last_map_value != map_value || last_rudder_value != map_rudder_value){
    last_sent = now;
    last_map_value = map_value;
    last_rudder_value = map_rudder_value;

    //Convert these values from 0 to 180
    //Print these values on the serial monitor
    Serial.println(map_value);  // Check the network regularly

    Serial.print(F("Sending... "));
    payload_t payload = { millis(), packets_sent++, map_value, map_rudder_value };
    bool ok = radio.write(&payload, payload_size);
    Serial.println(ok ? F("ok.") : F("failed."));
  }
}
