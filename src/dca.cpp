#include "main.h"
#include "dca.hpp"

double MotorPriority[5] = {0,0,0,0,0};

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
    int mn[5] = {8, 2, 2, 2, 1};
    double max = 0;
    while (true) {

        for (int i = 0; i < 5; i++) {
            //max += MotorPriority[i];
        }

        for (int i = 0; i < 5; i++) {
            // allocation is equal to 20A * (prio / total_prio) / (motors per system)
            // the more systems are running the higher the total_prio.
            // the system with the most prio should get the most amount of current to be allocated to that system.
            c[i] = (19000 * (MotorPriority[i]) / mn[i]);
        }

        setDriveCurrent(c[DRIVE]);
        setArmCurrent(c[ARM]);
        setMogoLiftCurrent(c[MOGO]);
        setOneBarCurrent(c[ONE]);
        setIntakeCurrent(c[INTAKE]);
        delay(20);

        //printf("d[%5.0f] a[%5.0f] m[%5.0f] o[%5.0f] i[%5.0f] max[%5.0f]\n", c[0], c[1], c[2], c[3], c[4], max);
    }
}