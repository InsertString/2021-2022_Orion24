#include "main.h"
#include "auto_functions.h"

#define X 0
#define Y 1
#define THETA 2

void move_to_point(Vector2D target, double heading, double timeout, double kp[3], double ki[3], double kd[3]) {
    // define the global error
    Vector2D globalError = target - GlobalPosition;
    // the target of the robot based on its rotational heading
    Vector2D localTarget = target.getHeadingBased(global_angle);
    // the current position of the robot based on its rotational heading
    Vector2D localCurrent = GlobalPosition.getHeadingBased(global_angle);
    // heading error
    double heading_error = heading - imu.get_rotation();
    // exit timer
    Timer exit_timer;
    // pid for the drive base
    PID drivePidX;
    PID drivePidY;
    PID drivePidTheta;

    // config PIDs
    drivePidX.set_PID_variables(localTarget.x, 127, -127, 0);
    drivePidY.set_PID_variables(localTarget.y, 127, -127, 0);
    drivePidTheta.set_PID_variables(heading, 127, -127, 0);

    drivePidX.set_PID_constants(kp[X], ki[X], kd[X]);
    drivePidY.set_PID_constants(kp[Y], ki[Y], kd[Y]);
    drivePidTheta.set_PID_constants(kp[THETA], ki[THETA], kd[THETA]);

    // loop as long as the robot is more than 1 cm away from the target point and is within the time limit
    while (globalError.getLength() > 1 && fabs(heading_error) > 0.5 && exit_timer.delta_time() < timeout) {
        // update vectors and errors
        globalError = target - GlobalPosition;
        localTarget = target.getHeadingBased(global_angle);
        localCurrent = GlobalPosition.getHeadingBased(global_angle);
        heading_error = heading - imu.get_rotation();

        // set motor power
        double x_power = drivePidX.output(localCurrent.x);
        double y_power = drivePidY.output(localCurrent.y);
        double theta_power = drivePidTheta.output(imu.get_rotation());

        power_drive(x_power, y_power, theta_power);
    }

    power_drive(0,0,0);
}
