#include "BARO.h"
Adafruit_MPL3115A2 baro;

const float seaLevelPressure =  1028.0; // hPa //needs config
float startingAltitude;
myBaro::myBaro(){}; //emtpy constructor
\
void myBaro::baroStart(){
  //waits until baro has started to continue
  if(!baro.begin()){
    Serial.println("Error: Barometer couldn't start"); 
    while(1){delay(10);};
  }
  delay(10);
  baro.setMode(MPL3115A2_ALTIMETER);

  Serial.println("Barometer started. Zeroing...");
  zeroBaro();
  Serial.println("Barometer Zeroed");
  Serial.println("Barometer setup done");
};

void myBaro::zeroBaro(){
  baro.setSeaPressure(seaLevelPressure);
  const int samples = 1;
  float sumAlt = 0;
  for (int i = 0; i < samples; i++) {
    sumAlt += baro.getAltitude();
    
    delay(5);
    
  }
  startingAltitude = sumAlt/samples;

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
  
  

  data.ASLAltitude = baro.getAltitude();
  data.AGLAltitude = data.ASLAltitude - startingAltitude;



  return;
};

//returns temperature
float myBaro::getBaroTemp(){
  return baro.getTemperature();
};
