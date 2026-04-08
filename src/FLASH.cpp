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
  char logBuffer[512]; // Smaller buffer since we aren't adding labels
snprintf(logBuffer, sizeof(logBuffer),
    "%.2f,%.2f,%.0f,%.3f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.4f,%.4f,%.4f,%.4f,%.2f,%.6f,%.6f,%.0f,%.2f,%.2f,%.2f\n",
    data.ms, data.batteryVoltage, data.fState, data.loopTime,
    data.ax, data.ay, data.az, 
    data.worldAx, data.worldAy, data.worldAz,
    data.gx, data.gy, data.gz,
    data.reltoglobeQ0, data.reltoglobeQ1, data.reltoglobeQ2, data.reltoglobeQ3,
    data.baroAltitude, data.lat, data.lng, data.sats,
    data.kal_X_pos, data.kal_X_vel, data.kal_X_accel
);
dataFile.print(logBuffer);
}  


void myFlash::printToSerial() {
  char buffer[1024];
  snprintf(buffer, sizeof(buffer),
    "\n--- SYSTEM DATA ---\n"
    "Time: %.2f ms | Batt: %.2f V | State: %.1f\n"
    "Loop: %.3f ms (Prev: %.3f)\n"
    "\n[IMU Acceleration]\n"
    "Raw:  X:%6.2f Y:%6.2f Z:%6.2f\n"
    "World: X:%6.2f Y:%6.2f Z:%6.2f\n"
    "\n[IMU Gyro & Orientation]\n"
    "Gyro: X:%6.2f Y:%6.2f Z:%6.2f\n"
    "Quat: [%.2f, %.2f, %.2f, %.2f]\n"
    "\n[Barometer]\n"
    "Alt: %.2f m | Bias: %.2f m\n"
    "\n[GPS Data]\n"
    "Lat/Lng: %.6f, %.6f | Sats: %.0f | HDOP: %.2f\n"
    "DateTime: %s %s\n"
    "Local: X:%.2f Z:%.2f Alt:%.2f (Bias: %.2f)\n"
    "\n[Kalman Filtered]\n"
    "X: P:%.2f V:%.2f A:%.2f\n"
    "Y: P:%.2f V:%.2f A:%.2f\n"
    "Z: P:%.2f V:%.2f A:%.2f\n"
    "-------------------\n",
    data.ms, data.batteryVoltage, data.fState,
    data.loopTime, data.prevLoopTime,
    data.ax, data.ay, data.az,
    data.worldAx, data.worldAy, data.worldAz,
    data.gx, data.gy, data.gz,
    data.reltoglobeQ0, data.reltoglobeQ1, data.reltoglobeQ2, data.reltoglobeQ3,
    data.baroAltitude, data.biasAltitude,
    data.lat, data.lng, data.sats, data.hdop,
    data.gpsDate.c_str(), data.gpsTime.c_str(),
    data.gpsx, data.gpsz, data.gpsAltitude, data.gps_altitude_bias,
    data.kal_X_pos, data.kal_X_vel, data.kal_X_accel,
    data.kal_Y_pos, data.kal_Y_vel, data.kal_Y_accel,
    data.kal_Z_pos, data.kal_Z_vel, data.kal_Z_accel
);

// 3. Print to Serial
Serial.print(buffer);
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









