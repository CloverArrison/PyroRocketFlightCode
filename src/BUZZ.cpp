#include "BUZZ.h"

myBuzz::myBuzz(int thePin) : Buzzer(thePin){}

void myBuzz::buzzIdle() {
  sound(NOTE_E4, 100);
  sound(NOTE_F4, 100);
}

void myBuzz::buzzStart() {
  sound(NOTE_A4, 200);
  sound(NOTE_D5, 300);
}

void myBuzz::buzzGPSfix() {
  sound(NOTE_C3, 100);
  sound(NOTE_C4, 200);
}

void myBuzz::initBuzzer() {
  sound(NOTE_F4, 200);
  sound(NOTE_F4, 200);
}
void myBuzz::buzzerError() {
  sound(NOTE_F5, 1000);
  delay(100);
  sound(NOTE_F5, 1000);
  delay(100);
  sound(NOTE_F5, 1000);
  delay(100);
}
void myBuzz::buzzComplete() {
  sound(NOTE_C4, 200);
  sound(NOTE_F4, 200);
}
void myBuzz::buzz1() {
}
void myBuzz::buzzLongs() {
}

void myBuzz::buzzOff() {
}
void myBuzz::buzzFast() {
}
