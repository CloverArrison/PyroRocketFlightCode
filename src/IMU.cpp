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

  data.gx = g.gyro.x;
  data.gy = g.gyro.y;
  data.gz = g.gyro.z;
  
  data.ax = a.acceleration.x;
  data.ay = a.acceleration.y;
  data.az = a.acceleration.z;

  data.accelMag = sqrt(data.ax * data.ax +data.ay * data.ay +data.az * data.az);
}

void myIMU::zeroGyro(){
  
  //also need to zero gyro rates
  
 

}

Madgwick filter; //Specific filter for IMU
float axGrav, ayGrav, azGrav;
float gxGrav, gyGrav, gzGrav;


void myIMU::IMUfilter() {
  //all work done here

  
  const double EARTHGRAVACC = 9.80665;

  axGrav = data.ax / EARTHGRAVACC;
  ayGrav = data.ay / EARTHGRAVACC;
  azGrav = data.az / EARTHGRAVACC;
  gxGrav = data.gx / EARTHGRAVACC;
  gyGrav = data.gy / EARTHGRAVACC;
  gzGrav = data.gz / EARTHGRAVACC;

  filter.updateIMU(gxGrav, gyGrav, gzGrav, axGrav, ayGrav, azGrav);

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