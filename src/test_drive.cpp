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

Vector2D unit_controller_target;
Vector2D resultant;
Vector2D unit_resultant;
double delta_theta;
double turn_power;

void relativeDriveControl() {
    // calculate the direction the controller wants the robot to go in
    unit_controller_target.x = master.get_analog(ANALOG_LEFT_X) / 127.0f;
    unit_controller_target.y = master.get_analog(ANALOG_LEFT_Y) / 127.0f;

    // display unit_contorller_targets on screen
    lcd::print(0, "uct.x[%0.2f], uct.y[%0.2f]",unit_controller_target.x, unit_controller_target.y);
    printf("uct.x[%0.2f], uct.y[%0.2f], delta_theta[%0.2f], r.x[%4.0f], r.y[%4.0f], turn[%4.0f]\n",unit_controller_target.x, unit_controller_target.y, delta_theta, resultant.x, resultant.y, turn_power);

    // difference in angle is target heading - current heading
    delta_theta = unit_controller_target.getAngle() - imu.get_heading();

    // display delta_theta
    lcd::print(1, "delta_theta[%0.2f]", delta_theta);

    // the unit vector for the componets of the target from the robots perspective
    unit_resultant.x = sin(delta_theta);
    unit_resultant.y = cos(delta_theta);

    // multiply the unit vector by 127 to get the scaled motor powers for x and y
    resultant = unit_resultant * 127;

    // turning will be independant to direction and serves to steer robot direction
    turn_power = master.get_analog(ANALOG_RIGHT_X);

    // display resultant
    lcd::print(2, "r.x[%4.0f], r.y[%4.0f], turn[%4.0f]", resultant.x, resultant.y, turn_power);

    // apply motor powers
    power_drive(resultant.x, resultant.y, turn_power);
}
