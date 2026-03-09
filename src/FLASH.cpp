#include "FLASH.h"
int chipSelect;
File dataFile;

myFlash::myFlash(int theCS){
  chipSelect = theCS;
}

void myFlash::flashSetup(String name){
  Serial.print("Initializing " + name + " card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this serial monitor after fixing your issue!");
    while (1);
  }
  Serial.println("initialization done.");
}

//private
bool myFlash::makeFile(String fileName){
  if (SD.exists(fileName + ".txt")){
    Serial.println("File exists Already.");
    return false;
  }
  else{
    dataFile = SD.open(fileName + ".txt", FILE_WRITE);
    dataFile.close();
    return true;
  }
}

//private
void myFlash::delFile(String fileName){
  if(SD.exists(fileName + ".txt")){
    SD.remove(fileName + "txt");
  }
  else{
    Serial.println("File does not exist: it can not be deleted");
  }
}

//private
void myFlash::writeDataFile(File dataFile){

    dataFile.printf("Time: %.2f ms, Loop Time: %.2f ms, Battery: %.2f V\n", data.ms, data.loopTime, data.batteryVoltage);

    dataFile.printf("Raw Accel: ax=%.3f, ay=%.3f, az=%.3f | Mag: %.3f\n", data.ax, data.ay, data.az, data.accelMag);
    dataFile.printf("World Accel: ax=%.3f, ay=%.3f, az=%.3f\n", data.worldAx, data.worldAy, data.worldAz);
    dataFile.printf("World Accel Bias: ax=%.3f, ay=%.3f, az=%.3f\n", data.worldAxBias, data.worldAyBias, data.worldAzBias);

    dataFile.printf("Gyro: gx=%.3f, gy=%.3f, gz=%.3f\n", data.gx, data.gy, data.gz);

    dataFile.printf("Orientation: Yaw=%.2f, Pitch=%.2f, Roll=%.2f | Bias: YawBias=%.2f, PitchBias=%.2f\n",
                    data.yaw, data.pitch, data.roll, data.yawBias, data.pitchBias);

    dataFile.printf("Altitude: Alt=%.2f, Baro=%.2f, BiasAlt=%.2f\n", data.altitude, data.baroAltitude, data.biasAltitude);

    dataFile.printf("Kalman: X_pos=%.3f, X_vel=%.3f, X_accel=%.3f\n", data.kal_X_pos, data.kal_X_vel, data.kal_X_accel);
    dataFile.printf("        X_posP=%.3f, X_velP=%.3f, X_accelP=%.3f\n", data.kal_X_posP, data.kal_X_velP, data.kal_X_accelP);
    dataFile.printf("        K1=%.3f, K2=%.3f, Inno=%.3f\n", data.kal_K_1, data.kal_K_2, data.kal_inno);

    dataFile.printf("PID Yaw: Kp=%.3f, Ki=%.3f, Kd=%.3f | Pitch/Z: Kp=%.3f, Ki=%.3f, Kd=%.3f\n",
                    data.kp_y, data.ki_y, data.kd_y, data.kp_z, data.ki_z, data.kd_z);

    dataFile.printf("GPS: lat=%.6f, lng=%.6f, x=%.3f, y=%.3f, alt=%.2f, bias=%.2f\n",
                    data.lat, data.lng, data.gpsx, data.gpsy, data.gpsAltitude, data.gps_altitude_bias);
    dataFile.printf("     sats=%.0f, hdop=%.2f\n", data.sats, data.hdop);

    dataFile.printf("State: %d, fState=%.2f\n", data.state, data.fState);
    dataFile.println("---------------------------------------------------------------------------------------------------------");
}  

void myFlash::printToSerial() {
  Serial.printf("Time: %.2f ms, Loop Time: %.2f ms, Battery: %.2f V\n", data.ms, data.loopTime, data.batteryVoltage);

  Serial.printf("Raw Accel: ax=%.3f, ay=%.3f, az=%.3f | Mag: %.3f\n", data.ax, data.ay, data.az, data.accelMag);
  Serial.printf("World Accel: ax=%.3f, ay=%.3f, az=%.3f\n", data.worldAx, data.worldAy, data.worldAz);
  Serial.printf("World Accel Bias: ax=%.3f, ay=%.3f, az=%.3f\n", data.worldAxBias, data.worldAyBias, data.worldAzBias);

  Serial.printf("Gyro: gx=%.3f, gy=%.3f, gz=%.3f\n", data.gx, data.gy, data.gz);

  Serial.printf("Orientation: Yaw=%.2f, Pitch=%.2f, Roll=%.2f | Bias: YawBias=%.2f, PitchBias=%.2f\n",
                data.yaw, data.pitch, data.roll, data.yawBias, data.pitchBias);
  Serial.println();
  Serial.printf("Altitude: Alt=%.2f, Baro=%.2f, BiasAlt=%.2f\n", data.altitude, data.baroAltitude, data.biasAltitude);

  Serial.printf("Kalman: X_pos=%.3f, X_vel=%.3f, X_accel=%.3f\n", data.kal_X_pos, data.kal_X_vel, data.kal_X_accel);
  Serial.printf("        X_posP=%.3f, X_velP=%.3f, X_accelP=%.3f\n", data.kal_X_posP, data.kal_X_velP, data.kal_X_accelP);
  Serial.printf("        K1=%.3f, K2=%.3f, Inno=%.3f\n", data.kal_K_1, data.kal_K_2, data.kal_inno);

  Serial.printf("PID Yaw: Kp=%.3f, Ki=%.3f, Kd=%.3f | Pitch/Z: Kp=%.3f, Ki=%.3f, Kd=%.3f\n",
                data.kp_y, data.ki_y, data.kd_y, data.kp_z, data.ki_z, data.kd_z);

  Serial.printf("GPS: lat=%.6f, lng=%.6f, x=%.3f, y=%.3f, alt=%.2f, bias=%.2f\n",
                data.lat, data.lng, data.gpsx, data.gpsy, data.gpsAltitude, data.gps_altitude_bias);
  Serial.printf("     sats=%.0f, hdop=%.2f\n", data.sats, data.hdop);

  Serial.printf("State: %d, fState=%.2f\n", data.state, data.fState);
  Serial.println("---------------------------------------------------------------------------------------------------------");
}

void myFlash::handleWriteFlash() {
  String fileName = data.gpsDate;
  if(makeFile(fileName)){
    writeDataFile(dataFile);
  }
  else{
    SD.open(fileName + ".txt", FILE_WRITE);
    writeDataFile(dataFile);
  }
}









