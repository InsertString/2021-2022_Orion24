#include "main.h"
#include "dca.hpp"

double MotorPriority[5] = {1,0,0,0,0};

void setDriveCurrent(double c) {
    driveLBL.set_current_limit(c);
    driveLBR.set_current_limit(c);
    driveLFL.set_current_limit(c);
    driveLFR.set_current_limit(c);
    driveRBL.set_current_limit(c);
    driveRBR.set_current_limit(c);
    driveRFL.set_current_limit(c);
    driveRFR.set_current_limit(c);
}

void setArmCurrent(double c) {
    ArmLeft.set_current_limit(c);
    ArmRight.set_current_limit(c);
}

void setMogoLiftCurrent(double c) {
    MogoLeft.set_current_limit(c);
    MogoRight.set_current_limit(c);
}

void setOneBarCurrent(double c) {

}

void setIntakeCurrent(double c) {
    Elevator.set_current_limit(c);
}

void dynamic_current_task(void * param) {
    double c[5] = {0, 0, 0, 0, 0};
    int mn[5] = {8, 2, 2, 0, 1};
    double max = 0;
    while (true) {
        // add up the max priority number
        max = 0;
        for (int i = 0; i < 5; i++) {
            max += MotorPriority[i];
        }

        for (int i = 0; i < 5; i++) {
            c[i] = (20 * MotorPriority[i] / max) / mn[i];
        }

        setDriveCurrent(c[DRIVE]);
        setArmCurrent(c[ARM]);
        setMogoLiftCurrent(c[MOGO]);
        setOneBarCurrent(c[ONE]);
        setIntakeCurrent(c[INTAKE]);
        delay(20);
    }
}