
// ^ Library includes
#include <Arduino.h>
#include <Wire.h>
#include <Chrono.h>

// ^ My includes
#include "IMU.h" 
#include "BARO.h"
#include "GPS.h"
#include "FLASH.h"
#include "FILTER.h"
#include "RADIO.h"
#include "DATA.h"
#include "STATES.h"
#include "BUZZ.h"
#include "CONFIG.h"
#include "BAT.h"

State state;
// ^public variables
// float accelMag = 0;                         
bool flashWriteStatus = false;                // Check if posable to write to flash
bool gyroZeroStatus = false;                  // Check if gyroscope has been zeroed
unsigned long landingDetectTime = 0;          // Running variable once landing has been triggered

bool finishedWriting = false;                 // Check if flash has finished write
unsigned long prevLoopTime;                   // Resets each next loop, measures loop time
unsigned long currentLoopTime;                // Resets each loop, measures loop time

bool firstLaunchLoop = true;                  // Is this the first loop in the launch commanded state    
bool firstAbortLoop = true;                   // Is this the first loop in the aborted state
// unsigned long abortLoopTime = 0;            // Unused

float firstPow = 0;                           // Is this the first loop in the powered assent state
float powStart = 0;                           // Measured time since powered assent start

bool isGPS0 = false;                          // Check if GPS has been zeroed

int mainSerialRate(115200);

// ^class objects
myIMU imu;                      // mpu object
myBaro barometer;               // baro object
myGPS gps;                      // GPS object
myFlash flash(flashPin);        // Flash object
myFlash sd(sdPin);              // SD object
myFilter filt;                  // Kalman filter object
myLoRa lora(radioPin);          // Lora object
myBuzz buzz(buzzPin);           // Buzz object
myBat bat(voltPin);             // Battery object
myFilter kalmanX;               // Kalman divided between axis
myFilter kalmanY;               // Y-axis is up
myFilter kalmanZ;

Chrono navTimer(Chrono::MICROS, false);                // Find nav timing (loop timeing)
Chrono loopTimer;

// ^ My functions
void handleNav();               // runs sensors logging, radio and state switching
bool isAnglePassedThreshold();  // checks if need to abort

// Set up sensors, output, Serial. Also inti states then switch to idle
void setup() {
  delay(1000);                    // Wait for feather to power on
  goToState(INITIALIZING);

  Serial.begin(mainSerialRate);           // main serial rate
  while (!Serial);
  Serial.print("Serial Rate: ");
  Serial.println(mainSerialRate);
  gps.GPSstart();                 // gps setup
  imu.IMUstart();                 // mpu setup
  barometer.baroStart();          // baro setup
  //sd.flashSetup("SD");            // sd setup
  //flash.flashSetup("Flash");      // Flash setup  // might be an error here?
  lora.LoRaStart();               // Radio setup

  kalmanX.startKalman();          // kalman setup 
  kalmanY.startKalman();
  kalmanZ.startKalman();
  
  // Read loop times
  prevLoopTime = 0;       
  currentLoopTime = micros();     

  // End setup choose state
  if (IS_TEST_MODE) {
    goToState(TEST);
    Serial.println("Going to test mode");
  } else {
    goToState(IDLE);
    Serial.println("Going to idle mode");

  }
  
  loopTimer.start();
  navTimer.start();
  Serial.println("end init");

  Serial.println("-------------------------------------------------------------------------");
}

void loop() {
  //Serial.println("loop");
  handleNav();                  // Get all sensor data, run filters, Check battery, run loop times
  //sd.handleWriteFlash();      
  //handleEUI();
  //handleTransmit();

  switch (data.state) {
    case INITIALIZING:  // should never be here, should switch out before
      {
        Serial.println("ERROR: INITIALIZING too late ");
        while(1);
      }

    case IDLE:
      {
        // Check if acceleration is high: if flight has started
        if (data.worldAy > LAUNCH_ACCEL_THRESHOLD) {
          goToState(POWERED_ASCENT);
          Serial.println("Going to POWERED_ASCENT mode");

        }
        break;
      }

    case LAUNCH_COMMANDED:
      {
        // If this is the first loop zero everything
        // firstLaunchLoop = true;
        if (firstLaunchLoop == true) {
          firstLaunchLoop = false;
          // setBaro0();
          //imu.zeroGyro();
          // zeroKalman();
          break;
        }

        // If accel is still high then motor is on go to pow ascent
        if (data.worldAy > LAUNCH_ACCEL_THRESHOLD) {
          goToState(POWERED_ASCENT);
          Serial.println("Going to POWERED_ASCENT mode");
        }
        break;
      }

    case POWERED_ASCENT:
      {
        if (firstPow == true) {
          firstPow = false;
          powStart = millis();
        }

        // Get acceleration: if it is less than 0 switch states: unpowered assent 
        data.accelMag = sqrt(sq(data.ax) + sq(data.ay) + sq(data.az));
        if (data.ax < ACCEL_UNPOWERED_THRESHOLD) {
          goToState(UNPOWERED_ASCENT);
          Serial.println("Going to UNPOWERED_ASCENT mode");
        }
        // Get velocity: if it is less than 0 switch states: free descent
        if (data.kal_X_vel <= -0.5f) {
          goToState(FREE_DESCENT);
          Serial.println("Going to FREE_DESCENT mode");
        }
        // Go to aport if angle is too far over
        if(isAnglePassedThreshold()){
          goToState(ABORT);
          Serial.println("Going to ABORT mode");
        } 
        break;
      }

    case UNPOWERED_ASCENT:
      {
        // Get velocity: if it is negative go to free descent
        if (data.kal_X_vel <= -0.5f) {
          goToState(FREE_DESCENT);
          Serial.println("Going to FREE_DESCENT mode");
        }
        break;
      }

    case FREE_DESCENT:
      {
        // Check for low enough altitude to deploy
        if (data.kal_Y_pos <= PARACHUTE_ALTITUDE_THRESHOLD) {
          // Write parachute launch to servo
          goToState(PARACHUTE_DESCENT);
          landingDetectTime = millis();
        }
        // Check time after pow start to deploy shoot: for safety
        if (powStart > PARACHUTE_EJECT_SAFETY_TIME)
        {
          goToState(ABORT);
          Serial.println("Going to ABORT mode");
        }
        break;
      }
      
    case PARACHUTE_DESCENT:
      {
        // pyroFire();
        // If velocity is near 0 go to landed
        if (data.kal_X_vel >= -0.5f) {
          goToState(LANDED);
          Serial.println("Going to LANDED mode");
        }
        break;
      }

    case LANDED:
      {
        // dump data
        break;
      }

    case ERROR:
      {
        buzz.buzzerError();
        delay(5000);
        break;
      }

    case ABORT:
      {
        // fire pyro?
        break;
      }

    case TEST:
      {

        break;
      }
    case GPS_BIAS_GATHER:
      {
        break;
      }
  }
  
  float prevMillis = data.ms;
  data.ms = millis();
  data.prevLoopTime = data.loopTime;
  data.loopTime = data.ms - prevMillis;

  loopTimer.restart();
}

void handleNav() {

  // get all data and write to data struct
  
  if (navTimer.hasPassed(NAV_RATE)) { 
    data.prevNavLoopTime = data.navLoopTime;
    data.navLoopTime = navTimer.elapsed();
    imu.getIMU();
    if(gps.isFix()){
      gps.GPSaltitude();
      gps.GPSx();
      gps.GPSz();
      gps.GPSlat();
      gps.GPSlon();
      gps.GPSsats();
    }
    
    // ^isnt getting passes get baro alt this needs to be fixed
    //barometer.baroAlt();

    
    sd.printToSerial();   // Prints formatted data to serial


    // bat.handleBatteryCheck();  // bat voltage

    

  //imu.IMUfilter();  // Take in raw imu data output the filtered attitude

  // Arrays for the output of kalman filter, blank to start
  float XfilteredDataArray[3];
  float YfilteredDataArray[3];
  float ZfilteredDataArray[3];

  // Kalman filter input is position and acceleration, array is blank to be edited by kalman
  kalmanX.runKalman(data.gpsx, data.ax, XfilteredDataArray);
  kalmanZ.runKalman(data.gpsz, data.az, ZfilteredDataArray);
  kalmanY.runKalman(data.gpsAltitude, data.ay, YfilteredDataArray);

  // Assign the output arrays from kalman to data
  data.kal_X_pos = XfilteredDataArray[0];
  data.kal_X_vel = XfilteredDataArray[1];
  data.kal_X_accel = XfilteredDataArray[2];
  data.kal_Z_pos = ZfilteredDataArray[0];
  data.kal_Z_vel = ZfilteredDataArray[1];
  data.kal_Z_accel = ZfilteredDataArray[2];
  data.kal_Y_pos = YfilteredDataArray[0];
  data.kal_Y_vel = YfilteredDataArray[1];
  data.kal_Y_accel = YfilteredDataArray[2];

  navTimer.restart();
  }
}

// Only run  in pow ascent
bool isAnglePassedThreshold() {
  if (ENABLE_ANGLE_CHECK == true) {
    if (abs(data.magYaw) >= ABORT_ANGLE_THRESHOLD || abs(data.magPitch) >= ABORT_ANGLE_THRESHOLD) {
      return true;
    }
  }
  return false;
}
