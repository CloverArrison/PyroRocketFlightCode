#include "IMU.h"
#include "DATA.h"
#include <MadgwickAHRS.h>
#include <Arduino.h>



Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

myIMU::myIMU(){};

void myIMU::IMUstart() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
    delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  //mpu settings
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

}

//GYRO
void myIMU::getIMU() {
  mpu.getEvent(&a, &g, &temp);

  data.gx = a.gyro.x;
  data.gy = a.gyro.y;
  data.gz = a.gyro.z;
  
  data.ax = a.acceleration.x;
  data.ay = a.acceleration.y;
  data.az = a.acceleration.z;

  data.accelMag = sqrt(data.ax * data.ax +data.ay * data.ay +data.az * data.az);
}

void myIMU::zeroGyro(){
  
  //also need to zero gyro rates
  
 

}

Madgwick filter;
float accelScale, gyroScale;

void myIMU::IMUfilter() {
  filter.updateIMU(data.gx, data.gy, data.gz, data.ax, data.ay, data.az);

  data.magYaw = filter.getRoll();
  data.magRoll = filter.getPitch();
  data.magPitch = filter.getYaw();

  data.reltoglobeQ0 = filter.getQ0();
  data.reltoglobeQ1 = filter.getQ1();
  data.reltoglobeQ2 = filter.getQ2();
  data.reltoglobeQ3 = filter.getQ3();
}

bool myIMU::convertToGlobal(){
  float qw = data.reltoglobeQ0;
  float qx = data.reltoglobeQ1;
  float qy = data.reltoglobeQ2;
  float qz = data.reltoglobeQ3;

  float ax = data.ax;
  float ay = data.ay;
  float az = data.az;

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