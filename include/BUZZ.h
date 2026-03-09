#ifndef BUZZ_H
#define BUZZ_H

#include "DATA.h"
#include "Buzzer.h"


class myBuzz : public Buzzer{

public:
  myBuzz(int thePin);     // Setup buzzer with pin
  void buzzIdle();        // Funtions to run short buzzers sounds
  void buzzStart();
  void buzzGPSfix();
  void initBuzzer();
  void buzzerError();
  void buzzComplete();
  void buzz1();
  void buzzLongs();
  void buzzOff();
  void buzzFast();
};
#endif
