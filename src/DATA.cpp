#include "DATA.h"
Data data;

void goToState(State state)
{
  data.state = state;
  data.fState = float(state);
}

void eraseFlightData()
{
//data = ();
}


