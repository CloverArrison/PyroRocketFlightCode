#ifndef RADIO_H
#define RADIO_H

#include <LoRa.h>
#include <SPI.h>
#include "DATA.h"

class myLoRa {

public:
  myLoRa(int LoraPin);               
  void LoRaStart();                  // Setup radio with pin
  void sendPacket(String packet);    // Send data transmit 
  String getPacket();                // Attempt to get data 
};


#endif