#pragma once

#include <Arduino.h>
#include "Chrono.h"
#include "DATA.h"

// Voltage Divider values
#define R1 1000000 //Resister 1
#define R2 100000  //Resister 2

// For flying, set threshold to 11.1 volts (medium)
class myBat {
public:
  myBat(int pin);             
  float battVoltage();        // Update battery voltage to data
  float getBattVoltage();     // Return battery voltage
  bool isBatteryLow();        // Check if battery voltage is low
  void handleBatteryCheck();  // Update battery per loop
};