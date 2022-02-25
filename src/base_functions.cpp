#include "main.h"

void power_drive(double y, double turn, double left, double right) {
    DriveLF = y + turn + left;
    DriveLM = y + turn + left;
    DriveLB = y + turn + left;
    DriveRF = y - turn + right;
    DriveRM = y - turn + right;
    DriveRB = y - turn + right;
}


void power_arm(double power) {
    ArmLeft = power;
    ArmRight = power;
}


void reset_arm_position() {
    ArmLeft.tare_position();
    ArmRight.tare_position();
}


void move_arm_to_position(double target, double speed) {
    ArmLeft.move_absolute(target, speed);
    ArmRight.move_absolute(target, speed);
}


void power_intake(double power) {
    Intake = power;
}


void power_mogo(double power) {
    MogoLeft = power;
    MogoRight = power;
}


void reset_mogo_position() {
    MogoLeft.tare_position();
    MogoRight.tare_position();
}


void move_mogo_to_position(double target, double speed) {
    MogoLeft.move_absolute(target, speed);
    MogoRight.move_absolute(target, speed);
}


void move_wrist(double power) {
    Wrist = power;
}


void debug_base_systems() {
    screen::print(TEXT_MEDIUM, 0, "DEBUG WINDOW");
    screen::print(TEXT_MEDIUM, 1, "LEFT MOGO: [%4.0f] RIGHT MOGO: [%4.0f]   ", MogoLeft.get_position(), MogoRight.get_position());
    if (MogoEndstop.get_value() == 1)
        screen::print(TEXT_MEDIUM, 2, "MOGO ENDSTOP: CLOSED");
    else 
        screen::print(TEXT_MEDIUM, 2, "MOGO ENDSTOP: OPEN  ");
    screen::print(TEXT_MEDIUM, 3, "LEFT ARM:  [%4.0f] RIGHT ARM:  [%4.0f]   ", ArmLeft.get_position(), ArmRight.get_position());
    if (ArmEndstop.get_value() == 1)
        screen::print(TEXT_MEDIUM, 4, "ARM ENDSTOP: CLOSED");
    else 
        screen::print(TEXT_MEDIUM, 4, "ARM ENDSTOP: OPEN  ");
}
