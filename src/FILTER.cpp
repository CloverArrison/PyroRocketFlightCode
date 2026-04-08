#include "CONFIG.h"
#include "FILTER.h"
#include <Arduino.h>
#include "DATA.h"


#define EKF_N 3 //pos vel acc
#define EKF_M 2 //pos acc
static const uint8_t LM35_PIN = 0;

#include <tinyekf.h>
#include <Wire.h>

float T = data.loopTime;
float DT = data.loopTime - data.prevLoopTime;

static const float EPS = 1e-4;

static const float Q[EKF_N*EKF_N] = { //3X3

    EPS, 0, 0,
    0, EPS, 0,   
    0, 0,  EPS
};

static const float R[EKF_M*EKF_M] = { //2X2

    EPS, 0,
    0,   EPS
    
};

// So process model Jacobian is identity matrix
static const float F[EKF_N*EKF_N] = { //3X3
    1, DT, 0.5 * DT * DT,
    0, 1,       DT,
    0, 0,       1
};

static const float H[EKF_M*EKF_N] = { //2X3
    1, 0, 0,
    0, 0, 1
};

static ekf_t _ekf;


myFilter::myFilter(){
        
}

void myFilter::startKalman(){
        const float Pdiag[EKF_N] = {1, 1};
        ekf_initialize(&_ekf, Pdiag);
}       

void myFilter::runKalman(float mesPos, float mesAcc, float dataOutArray[3]){
        //set obs vector
        const float z[EKF_M] = {mesPos, mesAcc};
        // process model
        const float fx[EKF_N] = { _ekf.x[0], _ekf.x[1] };
        // prediction step
        ekf_predict(&_ekf, fx, F, Q);
        // unknown/magic
        const float hx[EKF_M] = {_ekf.x[0], _ekf.x[1]};
        // update step
        ekf_update(&_ekf, z, hx, H, R);

        dataOutArray[0] = _ekf.x[0];
        dataOutArray[1] = _ekf.x[1];
        dataOutArray[2] = _ekf.x[2];
}


