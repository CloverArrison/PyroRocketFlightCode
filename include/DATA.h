#pragma once
#include <Arduino.h>
#include "STATES.h"
#include "FLASH.h"

void goToState(State state);

void eraseFlightData();

struct Data {
  float ms;
  float ax;
  float ay;
  float az;
  float worldAx;
  float worldAy;
  float worldAz;
  float gx;
  float gy;
  float gz;
  float yaw;    //x
  float pitch;  //z
  float roll;   //y
  float altitude;
  float baroAltitude;
  float biasAltitude;
  float loopTime;
  State state;
  float fState;
  float kal_X_pos;
  float kal_X_vel;
  float kal_X_accel;
  float kal_X_posP;
  float kal_X_velP;
  float kal_X_accelP;
  float kal_K_1;
  float kal_K_2;
  float kal_inno;
  float kp_y;
  float ki_y;
  float kd_y;
  float kp_z;
  float ki_z;
  float kd_z;
  float yawBias;
  float pitchBias;
  float batteryVoltage;
  float lat;
  float lng;
  float gpsx;
  float gpsy;
  float gpsAltitude;
  float gps_altitude_bias;
  String gpsDate;
  String gpsTime;
  float sats;
  float hdop;
  float worldAxBias;
  float worldAyBias;
  float worldAzBias;
  float accelMag;
  
};

extern Data data;