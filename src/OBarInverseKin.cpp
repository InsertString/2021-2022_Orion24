#include "main.h"
#include "OBarInverseKin.hpp"

double OBarAngle() {
    double temp = 0;
    temp = (OBarPot.get_value() - OBAR_POT_VERTICAL) / (4095.0f/ 230.0f);
    return temp;
}

double WristAngle() {
    return 0;
}

Vector2D WristPosition() {
    Vector2D temp;
    temp.y = OBAR_LENGTH_TO_PIVOT * sin(OBarAngle());
    temp.x = OBAR_LENGTH_TO_PIVOT * cos(OBarAngle());
    return temp;
}

PID OBarPid;

bool OBarMoveToPosition(double angle) {
    bool AtPosition = fabs(angle - OBarAngle()) < 1;
    OBarPid.set_PID_variables(angle, 100, -100, 0);
    OBarPid.set_PID_constants(4, 0, 10);
    OArm = OBarPid.output(OBarAngle());
    return AtPosition;
}
