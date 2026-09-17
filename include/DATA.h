                        #pragma once
#include <Arduino.h>
#include "STATES.h"
#include "FLASH.h"

void goToState(State state);

void eraseFlightData();

struct Data {
  float ms; // millis
  float navMS; // handle nav runs millis

  float ax; // mps
  float ay; // mps
  float az; // mps
  
  //float kalWorldAx;
  //float kalWorldAy;
  //float kalWorldAz;

  float worldAx; // mps
  float worldAy; // mps
  float worldAz; // mps

  float gx; // deg/sec
  float gy; // deg/sec
  float gz; // deg/sec
  float gxBias; // deg/sec
  float gyBias; // deg/sec
  float gzBias; // deg/sec
  float magYaw;    // x deg
  float magPitch;  // z deg
  float magRoll;   // y deg


  // Quaternions 
  float reltoglobeQ0;
  float reltoglobeQ1;
  float reltoglobeQ2;
  float reltoglobeQ3;

  float ASLAltitude; // meters above sea level
  float AGLAltitude; // meters above ground

  float loopTimeMicros = 0; // ms
  float prevLoopTimeMicros = 0; // ms 
  float navLoopTimeMicros = 0; // microsecond
  float prevNavLoopTimeMicros = 0; // microsecond

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

  float lat; // deg
  float lng; // deg
  float gpsx; // m
  float gpsz; // m
  float gpsAltitude; // m?
  float gps_altitude_bias; // m?

  String gpsDate;
  String gpsTime;
  float sats;
  float hdop;

  // 
  float worldAxBias;
  float worldAyBias;
  float worldAzBias;

  float accelMag;
  
};

extern Data data;
