#ifndef _OBARINVERSEKIN_HPP_
#define _OBARINVERSEKIN_HPP_

#include "Vector2D.hpp"

#define OBAR_POT_VERTICAL 2000
#define OBAR_POT_INITIAL 500
#define OBAR_POT_MAX 4000
#define OBAR_LENGTH_TO_PIVOT 43.18

double OBarAngle();
double WristAngle();

Vector2D WristPosition();

#endif