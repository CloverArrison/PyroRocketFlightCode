#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "DATA.h"

class myIMU {
public:
  myIMU();
  void IMUstart();    // Setup with all settings
  void getIMU();      // Read IMU print data
};

#endif