#include "main.h"

Timer::Timer() {
  start_time = 0;
}

void Timer::reset() {
  start_time = millis();
}

double Timer::delta_time() {
  return (millis() - start_time);
}
