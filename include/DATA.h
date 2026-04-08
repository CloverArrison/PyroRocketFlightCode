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
  
  //float kalWorldAx;
  //float kalWorldAy;
  //float kalWorldAz;

  float worldAx;
  float worldAy;
  float worldAz;

  float gx;
  float gy;
  float gz;
  float magYaw;    //x deg
  float magPitch;  //z deg
  float magRoll;   //y deg

  float reltoglobeQ0;
  float reltoglobeQ1;
  float reltoglobeQ2;
  float reltoglobeQ3;

  float baroAltitude;
  float biasAltitude;

  float loopTime;
  float prevLoopTime;

  State state;
  float fState;

  float kal_X_pos;
  float kal_X_vel;
  float kal_X_accel;
  float kal_Z_pos;
  float kal_Z_vel;
  float kal_Z_accel;
  float kal_Y_pos;
  float kal_Y_vel;
  float kal_Y_accel;

  /*
  float kal_K_1;
  float kal_K_2;
  float kal_inno;
  float kp_y;
  float ki_y;
  float kd_y;
  float kp_z;
  float ki_z;
  float kd_z;
  */
  float yawBias;
  float pitchBias;

  float batteryVoltage;

  float lat;
  float lng;
  float gpsx;
  float gpsz;
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