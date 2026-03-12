#include "WString.h"
#ifndef GPS_H
#define GPS_H

#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "DATA.h"

#define GPSSerial Serial1


class myGPS {

public:
  myGPS();

  void GPSstart();        // Set up connection to GPS
  bool isFix();           // Test if GPS has a fix
  void setGPS0();         // Set zero position from GPS data
  void GPSdata();         // Collect all GPS data
  String GPSdate();       // Output data from GPS as a string
  String GPStime();       // Output time from GPS as a string
  void GPSaltitude();     // Write altitude to data
  void GPSx();            // Write GPS x cord to data
  void GPSy();            // Write GPS y cord to data
  void GPSlat();          // Write GPS latitude cord to data
  void GPSlon();          // Write GPS longitude cord to data
  void GPSsats();         // Find how many santilites 
  void GPSupdate();       // Read new data from GPS
};


#endif