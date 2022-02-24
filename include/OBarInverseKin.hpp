#ifndef _OBARINVERSEKIN_HPP_
#define _OBARINVERSEKIN_HPP_

#define OBAR_POT_VERTICAL 2900
#define OBAR_POT_INITIAL 760
#define OBAR_POT_MAX 4000
#define OBAR_LENGTH_TO_PIVOT 43.18

double OBarAngle();
double WristAngle();

Vector2D WristPosition();

// moves OBar to position and returnstrue when within 2 degrees of the spsific angle
bool OBarMoveToPosition(double angle);

#endif