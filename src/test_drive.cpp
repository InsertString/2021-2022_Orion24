#include "main.h"
#include "test_drive.h"

void power_drive(double x, double y, double turn) {
    driveFL =  y + turn;
    driveBL =  y + turn;
    driveFR = -y + turn;
    driveBR = -y + turn;
    driveSF =  x + (turn * STRAFE_TURN_CONST);
    driveSB = -x + (turn * STRAFE_TURN_CONST);
}

Vector2D controller_target;
Vector2D resultant;
Vector2D unit_resultant;
double delta_theta;
double turn_power;

void relativeDriveControl() {
    // calculate the direction the controller wants the robot to go in
    controller_target.x = master.get_analog(ANALOG_LEFT_X);
    controller_target.y = master.get_analog(ANALOG_LEFT_Y);
    
}
