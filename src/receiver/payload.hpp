/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 * **************************************************************************************
 * ANY CHANGE ON THIS FILE SHOULD BE MANUALY SYNCED BETWEEN transmitter and receiver CODE
 * **************************************************************************************
 */

//TODO: na to ylopoiisw me uint16_t anti gia int wste na eimaste panta sigouroi
//gia to payload size, aneksartita apo tin ylopoiisi sto hardware
struct payload_t {  // Structure of our payload
  unsigned int throttle;
  unsigned int rudder;
  unsigned int elevator;
  unsigned int aileron;
};

const uint8_t PAYLOAD_SIZE = sizeof(struct payload_t);