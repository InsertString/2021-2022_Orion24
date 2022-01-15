#include "main.h"
#include "OBarInverseKin.hpp"

double OBarAngle() {
    double temp = 0;
    temp = (OBarPot.get_value() - OBAR_POT_VERTICAL) / (230 / 4095);
    return temp;
}

double WristAngle() {
    
}

Vector2D WristPosition() {
    Vector2D temp;
    temp.y = OBAR_LENGTH_TO_PIVOT * sin(OBarAngle());
    temp.x = OBAR_LENGTH_TO_PIVOT * cos(OBarAngle());
    return temp;
}
