#include "BARO.h"
Adafruit_MPL3115A2 baro;

float seaLevelPressure = 1013.26; //needs config
myBaro::myBaro(){}; //emtpy constructer

void myBaro::baroStart(){
  //waits until baro has started to continue
  if(!baro.begin()){
    Serial.println("Error: Berometer couldn't start"); 
    while(1){delay(10);};
  }
   
  baro.setSeaPressure(seaLevelPressure); //set sea level press depending on location
  Serial.println("Baro setup done");
};

//returns pressure
float myBaro::getBaroPress(){
  return baro.getPressure();
};

//returns altitude
float myBaro::getBaroAlt(){
  return baro.getAltitude();
};

//updates data with altitude
void myBaro::baroAlt(){
  data.baroAltitude = baro.getAltitude();
  return;
};

//returns temperature
float myBaro::getBaroTemp(){
  return baro.getTemperature();
};
