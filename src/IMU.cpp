#include "IMU.h"
#include "DATA.h"
#include <MadgwickAHRS.h>
#include <Arduino.h>

#include "CONFIG.h"


Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
Madgwick filter;


myIMU::myIMU(){};

void myIMU::IMUstart() {
  mpu.begin();
  delay(50);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
    delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  //mpu settings
  //mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  //mpu.setMotionDetectionThreshold(1);
  //mpu.setMotionDetectionDuration(20);
  //mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  //mpu.setInterruptPinPolarity(true);
  //mpu.setMotionInterrupt(true);
  


  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);


  filter.begin(NAV_RATE);



}

//GYRO
void myIMU::getIMU() {
  // units m/s^2, rad/s, deg C
  mpu.getEvent(&a, &g, &temp);

  data.gx = g.gyro.x;
  data.gy = g.gyro.y;
  data.gz = g.gyro.z;
  
  data.ax = a.acceleration.x;
  data.ay = a.acceleration.y;
  data.az = a.acceleration.z;

  data.accelMag = sqrt(data.ax * data.ax +data.ay * data.ay +data.az * data.az);
  IMUfilter();
}

void myIMU::zeroGyro(){
  
  //also need to zero gyro rates
  
 

}

float axGrav, ayGrav, azGrav;
float gxDeg, gyDeg, gzDeg;


void myIMU::IMUfilter() {
  //all work done here

  const double EARTHGRAVACC = 9.80665;
  const double RAD2DEG = 57.29578;

  //convert m/s^2 to g's for the filter
  axGrav = data.ax / EARTHGRAVACC;
  ayGrav = data.ay / EARTHGRAVACC;
  azGrav = data.az / EARTHGRAVACC;
  // Convert radians to degrees 
  gxDeg = data.gx * RAD2DEG; 
  gyDeg = data.gy * RAD2DEG;
  gzDeg = data.gz * RAD2DEG;


    
  
  filter.updateIMU(gxDeg, gyDeg, gzDeg, axGrav, ayGrav, azGrav);
  
  // +y is up, +x is right, +z is out from the board
  // my roll is y, my pitch is z, my yaw is x

  // library uses roll pitch yaw x y z, I use yaw roll pitch x y z
  // assigns to data and accounts for difference
  data.magYaw = filter.getRoll();
  data.magRoll = filter.getPitch();
  data.magPitch = filter.getYaw();

  // Uses my own get function in library
  // Normally these are hidden as private
  data.reltoglobeQ0 = filter.getQ0();
  data.reltoglobeQ1 = filter.getQ1();
  data.reltoglobeQ2 = filter.getQ2();
  data.reltoglobeQ3 = filter.getQ3();

  convertToGlobal();

}

void myIMU::convertToGlobal(){
  // Short hand for readability 
  float qw = data.reltoglobeQ0;
  float qx = data.reltoglobeQ1;
  float qy = data.reltoglobeQ2;
  float qz = data.reltoglobeQ3;
  float ax = axGrav;
  float ay = ayGrav;
  float az = azGrav;

  // Uses the quaternion to rotate the acceleration from the IMU to a global reference frame
  data.worldAx = ax * (1.0f - 2.0f * qy * qy - 2.0f * qz * qz) +
                    ay * (2.0f * qx * qy - 2.0f * qw * qz) +
                    az * (2.0f * qx * qz + 2.0f * qw * qy);

  data.worldAy = ax * (2.0f * qx * qy + 2.0f * qw * qz) +
                    ay * (1.0f - 2.0f * qx * qx - 2.0f * qz * qz) +
                    az * (2.0f * qy * qz - 2.0f * qw * qx);

  data.worldAz = ax * (2.0f * qx * qz - 2.0f * qw * qy) +
                    ay * (2.0f * qy * qz + 2.0f * qw * qx) +
                    az * (1.0f - 2.0f * qx * qx - 2.0f * qy * qy);

}