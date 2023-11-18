/**
 * Copyright (C) 2023 Technology Club Of Thrace <info@steth.gr>
 *
 * **************************************************************************************
 * ANY CHANGE ON THIS FILE SHOULD BE MANUALY SYNCED BETWEEN transmitter and receiver CODE
 * **************************************************************************************
 */

const byte PIPE_ADDRESS[5] = {'S', 'T', 'T', 'H', '1'};

struct payload_t {  // Structure of our payload
  unsigned long  ms;
  unsigned long counter;
  unsigned int  elevator;
  unsigned int  rudder;
  // unsigned float BatteryVoltage;
  // unsigned float x,y,z;
};

const uint8_t PAYLOAD_SIZE = sizeof(struct payload_t);