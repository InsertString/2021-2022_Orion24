#include "main.h"

void power_drive_differential(double left, double right) {
    DriveL1 = left;
    DriveL2 = left;
    DriveL3 = left;
    DriveL4 = left;
    DriveR1 = right;
    DriveR2 = right;
    DriveR3 = right;
    DriveR4 = right;
}

void power_drive_directional(double y, double turn) {
    DriveL1 = y + turn;
    DriveL2 = y + turn;
    DriveL3 = y + turn;
    DriveL4 = y + turn;
    DriveR1 = y - turn;
    DriveR2 = y - turn;
    DriveR3 = y - turn;
    DriveR4 = y - turn;
}

void tare_drive_motors() {
    DriveL1.tare_position();
    DriveL2.tare_position();
    DriveL3.tare_position();
    DriveL4.tare_position();
    DriveR1.tare_position();
    DriveR2.tare_position();
    DriveR3.tare_position();
    DriveR4.tare_position();
}

double drive_position() {
    double left_av = 0;
    double left_deg = 0;
    double left_cm = 0;
    double right_av = 0;
    double right_deg = 0;
    double right_cm = 0;
    
    // calculate the average position of the motor encoders on the left side
    left_av += DriveL1.get_position();
    left_av += DriveL2.get_position();
    left_av += DriveL3.get_position();
    left_av += DriveL4.get_position();
    left_av /= 4.0;
    
    // convert the value to degrees
    left_deg = (left_av / 900.0) * 360.0;

    // convert the value to cm
    left_cm = convert_drive_deg_to_cm(left_deg);

    // calculate the average position of the motor encoders on the left side
    right_av += DriveR1.get_position();
    right_av += DriveR2.get_position();
    right_av += DriveR3.get_position();
    right_av += DriveR4.get_position();
    right_av /= 4.0;
    
    // convert the value to degrees
    right_deg = (right_av / 900.0) * 360.0;

    // convert the value to cm
    right_cm = convert_drive_deg_to_cm(right_deg);

    return (right_cm + left_cm) / 2.0;
}

double convert_drive_deg_to_cm(double input) {
    return (input * 3.1415 / 180) * 1.25 * in_to_cm(4);
}

double in_to_cm(double in) {
    return in * 2.54;
}

void power_conveyor(double left, double right) {
    LeftConveyor = left;
    RightConveyor = right;
}

PID stab_PID;

void stabalize(PIDConstants k, double max, double int_limit) {
    stab_PID.set_constants(k);
    stab_PID.set_variables(-6, max, -max, int_limit);

    power_drive_directional(-stab_PID.output(imu.get_roll()), 0);
}


void engage_drive_brake() {
    DriveL1.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveL2.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveL3.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveL4.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveR1.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveR2.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveR3.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    DriveR4.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}

void disengage_drive_brake() {
    DriveL1.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveL2.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveL3.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveL4.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveR1.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveR2.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveR3.set_brake_mode(E_MOTOR_BRAKE_COAST);
    DriveR4.set_brake_mode(E_MOTOR_BRAKE_COAST);
}

void path_to_point(Vector2D target) {
    
}
