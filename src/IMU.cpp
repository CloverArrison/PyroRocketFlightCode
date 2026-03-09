#include "IMU.h"
#include "DATA.h"


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
}
