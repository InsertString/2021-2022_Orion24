#include "main.h"

void power_drive(double x, double y, double turn) {
    driveFL =  y + turn;
    driveBL =  y + turn;
    driveFR = -y + turn;
    driveBR = -y + turn;
    driveSF =  x + (turn * STRAFE_TURN_CONST);
    driveSB = -x + (turn * STRAFE_TURN_CONST);
}

Vector2D unit_controller_target;
Vector2D resultant;
Vector2D unit_resultant;
double delta_theta;
double turn_power;

void relativeDriveControl() {
    // calculate the direction the controller wants the robot to go in 
    unit_controller_target.x = master.get_analog(ANALOG_LEFT_X) / 127.0f;
    unit_controller_target.y = master.get_analog(ANALOG_LEFT_Y) / 127.0f;

    // difference in angle is target heading - current heading
    delta_theta = unit_controller_target.getAngle() - imu.get_heading();

    // the unit vector for the componets of the target from the robots perspective
    unit_resultant.x = sin(delta_theta);
    unit_resultant.y = cos(delta_theta);

    // multiply the unit vector by 127 to get the scaled motor powers for x and y
    resultant = unit_resultant * 127;

    // turning will be independant to direction and serves to steer robot direction
    turn_power = master.get_analog(ANALOG_RIGHT_X);

    // apply motor powers
    power_drive(resultant.x, resultant.y, turn_power);
}
