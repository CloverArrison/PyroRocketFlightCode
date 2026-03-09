#include "BAT.h"
#include "DATA.h"

Chrono batteryCheckTimer;
int Batpin;

myBat::myBat(int thePin){
  Batpin = thePin;
}

float myBat::getBattVoltage(){
  analogReadResolution(12);

  int reading = analogRead(9); // Raw ADC value
  float outVolts = reading * 3.3f/ 1024.0f; // Convert to actual voltage at analog pin

  return outVolts * 2.0f; // Compensate for voltage divider
}

void myBat::handleBatteryCheck()
{

  if (batteryCheckTimer.hasPassed(50))
  {
    data.batteryVoltage = getBattVoltage();
    batteryCheckTimer.restart();
  }
}

bool myBat::isBatteryLow()
{
  if (getBattVoltage() <= 3.3)
  {
    return true;
  }
  else
  {
    return false;
  }
}
