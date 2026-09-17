#include "IMU.h"

#include "DATA.h"

#include <MadgwickAHRS.h>
#include <Arduino.h>

#include "CONFIG.h"


Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
Madgwick filter;

const double EARTHGRAVACC = 9.80665;

myIMU::myIMU(){};

void myIMU::IMUstart() {
  mpu.begin();
  delay(100);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
    delay(10);
    }
  }

  mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 Found! Zeroing...");
  zeroGyro();
  Serial.println("IMU Zeroed");
  
  

  filter.begin(18.8679245283); // Start at ~20Hz for the 50ms nav rate
}

//GYRO
void myIMU::getIMU() {
  // units m/s^2, rad/s, deg C
  mpu.getEvent(&a, &g, &temp);

  data.gx = g.gyro.x - data.gxBias;
  data.gy = g.gyro.y - data.gyBias;
  data.gz = g.gyro.z - data.gzBias;
  
  data.ax = a.acceleration.x;
  data.ay = a.acceleration.y;
  data.az = a.acceleration.z;

  data.accelMag = sqrt(data.ax * data.ax + data.ay * data.ay +data.az * data.az);

  IMUfilter();
  convertToGlobal();
}

void myIMU::zeroGyro() {
  const int samples = 300;
  float sumX = 0, sumY = 0, sumZ = 0;
  
  for (int i = 0; i < samples; i++) {
    mpu.getEvent(&a, &g, &temp);
    sumX += g.gyro.x;
    sumY += g.gyro.y;
    sumZ += g.gyro.z;
    delay(2);
  }
  
  data.gxBias = sumX / samples;
  data.gyBias = sumY / samples;
  data.gzBias = sumZ / samples;
  
  // data.gx -= data.gxBias;
  // data.gy -= data.gyBias;
  // data.gz -= data.gzBias;
  
}

float axGrav, ayGrav, azGrav;

void myIMU::IMUfilter() {
  // this might not work, the filter doesn't say this works
  // I want to always use the best estimate of nav frequency
  // so this gets a new frequency every loop
  // it might not work since the examples show .begin only being for the first time the filter starts
  // it should work becasue the begin funtion only sets the frequency without any other logic
  // This still needs to be tested though


  // if acceliration too large turn off beta so Madgwick doesn't think the motor accel is gravity
  if(data.accelMag > EARTHGRAVACC * 1.5){
    filter.begin(1000000.0f / data.navLoopTimeMicros, 0.0001);
  }
  else{
    filter.begin(1000000.0f / data.navLoopTimeMicros);
  }
  
  // +y is up, +x is right, +z is out from the board
  // my roll is y, my pitch is z, my yaw is x

  //convert m/s^2 to g's for the filter
  axGrav = data.ax / EARTHGRAVACC;
  ayGrav = data.ay / EARTHGRAVACC;
  azGrav = data.az / EARTHGRAVACC;
  

  filter.updateIMU(data.gx, data.gy, data.gz, axGrav, ayGrav, azGrav);
  //filter.updateIMU_Rad_MPS2(data.gx, data.gy, data.gz, data.ax, data.ay, data.az);

  // library uses roll pitch yaw x y z, I use yaw roll pitch x y z
  // assigns to data and accounts for difference
  data.magYaw = filter.getRoll(); //degrees
  data.magRoll = filter.getPitch();
  data.magPitch = filter.getYaw();

  // Uses my own get function in library
  // Normally these are hidden as private
  data.reltoglobeQ0 = filter.getQ0();
  data.reltoglobeQ1 = filter.getQ1();
  data.reltoglobeQ2 = filter.getQ2();
  data.reltoglobeQ3 = filter.getQ3();
}

void myIMU::convertToGlobal(){
  // Short hand for readability 
  float qw = data.reltoglobeQ0;
  float qx = data.reltoglobeQ1;
  float qy = data.reltoglobeQ2;
  float qz = data.reltoglobeQ3;
  float ax = data.ax;
  float ay = data.ay;
  float az = data.az;

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