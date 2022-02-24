#include "main.h"
#include "OBarInverseKin.hpp"

double OBarAngle() {
    double temp = 0;
    int temp_int = OBarPot.get_value() / 10;
    temp = (temp_int * 10 - OBAR_POT_VERTICAL) / (4095.0f/ 230.0f);
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
    OBarPid.set_PID_variables(angle, 100, -100, 3);
    if (fabs(OBarAngle()) < 10) {
        OBarPid.set_PID_constants(1, 0.1, 10);
    }
    else {
        OBarPid.set_PID_constants(5, 0.02, 30);
    }
    OArm = OBarPid.output(OBarAngle());
    return AtPosition;
}
