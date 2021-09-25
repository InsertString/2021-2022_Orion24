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
    unit_controller_target.x = master.get_analog(ANALOG_LEFT_X) / 127.0;
    unit_controller_target.y = master.get_analog(ANALOG_LEFT_Y) / 127.0;

    // display unit_contorller_targets on screen
    lcd::print(0, "uct.x[%2.2f], uct.y[%2.2f]",unit_controller_target.x, unit_controller_target.y);
    printf("uct.x[%2.2f], uct.y[%2.2f], delta_theta[%2.2f], r.x[%4.0f], r.y[%4.0f], turn[%4.0f]\n",unit_controller_target.x, unit_controller_target.y, delta_theta, resultant.x, resultant.y, turn_power);

    // difference in angle is target heading - current heading
    // convert from degrees to radians
    if (fabs(unit_controller_target.x) > 0.1 || fabs(unit_controller_target.y) > 0.1)
      delta_theta = (unit_controller_target.getAngle() - imu.get_heading()) * 3.14159 / 180;
    else {
      delta_theta = 0;
    }

    // display delta_theta
    lcd::print(1, "delta_theta[%0.2f]", delta_theta);

    // the unit vector for the componets of the target from the robots perspective
    if (fabs(unit_controller_target.x) > 0.1 || fabs(unit_controller_target.y) > 0.1) {
      unit_resultant.x = sin(delta_theta);
      unit_resultant.y = cos(delta_theta);
    }
    else {
      unit_resultant.x = 0;
      unit_resultant.y = 0;
    }


    // multiply the unit vector by 127 to get the scaled motor powers for x and y
    resultant = unit_resultant * 127;

    // turning will be independant to direction and serves to steer robot direction
    turn_power = master.get_analog(ANALOG_RIGHT_X);

    // display resultant
    lcd::print(2, "r.x[%4.0f], r.y[%4.0f], turn[%4.0f]", resultant.x, resultant.y, turn_power);

    // apply motor powers
    power_drive(resultant.x, resultant.y, turn_power);
}
