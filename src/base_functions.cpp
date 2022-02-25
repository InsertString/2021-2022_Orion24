#include "main.h"

void power_drive(double y, double turn) {

}


void power_arm(double power) {

}


void move_arm_to_position(double target, double speed) {

}


void power_intake(double power) {
    Intake = power;
}


void power_mogo(double power) {
    MogoLeft = power;
    MogoRight = power;
}


void move_mogo_to_position(double target, double speed) {
    MogoLeft.move_absolute(target, speed);
    MogoRight.move_absolute(target, speed);
}


void debug_base_systems() {
    screen::print(TEXT_MEDIUM, 0, "DEBUG WINDOW");
    screen::print(TEXT_MEDIUM, 1, "LEFT MOGO: [%4.0f] RIGHT MOGO: [%4.0f]", MogoLeft.get_position(), MogoRight.get_position());
    if (MogoEndstop.get_value() == 1)
        screen::print(TEXT_MEDIUM, 2, "MOGO ENDSTOP: CLOSED");
    else 
        screen::print(TEXT_MEDIUM, 2, "MOGO ENDSTOP: OPEN  ");
    //screen::print(TEXT_MEDIUM, 2, "LEFT ARM:  [%4.0f] RIGHT ARM:  [%4.0f]");
}
