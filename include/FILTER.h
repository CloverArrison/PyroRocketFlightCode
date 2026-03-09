
#ifndef FILTER_H
#define FILTER_H

//#include <Kalman.h>
//#include "MadgwickAHRS.h"
//could use https://github.com/simondlevy/TinyEKF?utm_source=chatgpt.com insted?

//do one filter per sensor for loop time and storage cap

class myFilter {

public:
  myFilter();
  void startFilter();
};

#endif