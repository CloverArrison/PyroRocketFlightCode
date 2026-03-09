#ifndef BARO_H
#define BARO_H

#include <Adafruit_MPL3115A2.h>
#include "DATA.h"
#

class myBaro{
  public:
    myBaro();
    void baroStart();       // Set up barometer
    float getBaroPress();   // Output barometer pressure
    float getBaroAlt();     // Output altitude read barometer pressure
    void baroAlt();         // Writes altitude to data
    float getBaroTemp();    // Output barometer temperature
  private:


};
 
#endif