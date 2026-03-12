#include "CONFIG.h"
#include "FILTER.h"

#include <Arduino.h>
#include "DATA.h"
#include <Kalman.h>

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;
Kalman kalmanZ; 

double accX, accY, accZ;
double gyroX, gyroY, gyroZ;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;

myFilter::myFilter() {}

void myFilter::startFilter() {
        accX = data.worldAx;
        accY = data.worldAy;
        accZ = data.worldAz;
        

        kalmanX.setAngle(data.gx);
        kalmanY.setAngle(data.gy);

        gyroXangle = data.gx;
        gyroYangle = data.gy;
        compAngleX = data.gx;
        compAngleY = data.gy;
        

        timer = data.loopTime;

}

void myFilter::newData(){
        accX = data.ax;
        accY = data.ay;
        accZ = data.az;
        gyroX = data.gx;
        gyroY = data.gy;
        gyroZ = data.gz;

        timer = data.loopTime;
        double dt = data.loopTime - data.prevLoopTime;


}

