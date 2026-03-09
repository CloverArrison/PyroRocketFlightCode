//#include <cmath>
#include "WString.h"
#include "GPS.h"

#define GPSECHO false

Adafruit_GPS GPS(&GPSSerial);

uint32_t timer = millis();
float lat0;
float lon0;
float cosLat0;
static float earthr = 6.378 * (10 ^ 6);

myGPS::myGPS(){};

//start the GPS
void myGPS::GPSstart() {

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_BAUD_9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  GPS.sendCommand(LOCUS_OVERLAP);
  Serial.println("GPS init");
}
//set the 0 point for lat lon and alt
void myGPS::setGPS0() {
  lat0 = GPS.latitude;
  lon0 = GPS.latitude;
  cosLat0 = std::cos(lat0);
  data.gps_altitude_bias = GPS.altitude;
  Serial.println("set gps 0s");
}
//test if fix == true
bool myGPS::isFix() {
  if(GPS.fix){Serial.println("GPS Fixed");}
  return GPS.fix;
}

//for finding the date and time
/*
//return date
String myGPS::GPSdate() {
  return (String)GPS.month + ", " + (String)GPS.day + ", " + (String)GPS.year;
}
// return time
String myGPS::GPStime() {
  return (String)GPS.hour + ":" + (String)GPS.minute + ":" + (String)GPS.seconds + "." + (String)GPS.milliseconds;
}
*/

//return alt
void myGPS::GPSaltitude() {
  data.gpsAltitude = (float)GPS.altitude - data.gps_altitude_bias;
}
//return GPS x
void myGPS::GPSx() {
  data.gpsx = (GPS.longitude * cosLat0 * earthr);  // - (lon0* std::cos(cosLat0) * earthr);
}
//return GPS y
void myGPS::GPSy() {
  data.gpsy = (earthr * GPS.latitude) - (earthr * lat0);
}
//return GPS lat
void myGPS::GPSlat() {
  data.lat = GPS.latitude;
}
//return GPS lon
void myGPS::GPSlon() {
  data.lng = GPS.longitude;
}

void myGPS::GPSsats(){
  data.sats = GPS.satellites;
}

void myGPS::GPSupdate(){
  GPS.read();
  if (GPS.newNMEAreceived()) {
    Serial.print(GPS.lastNMEA());
    if (!GPS.parse(GPS.lastNMEA())) 
      return; 
  }
}


void myGPS::GPSdata() {
  GPS.hour;
  GPS.minute;
  GPS.seconds;
  GPS.milliseconds;
  GPS.fix;
  GPS.latitude;
  GPS.lat;
  GPS.longitude;
  GPS.lon;
  GPS.speed;
  GPS.angle;
  GPS.altitude;
  GPS.satellites;
  GPS.antenna;

}
