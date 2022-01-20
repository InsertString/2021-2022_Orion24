#include "main.h"
#include "auto_functions.h"

PID mwp_x_pid;
PID mwp_y_pid;
PID mwp_heading_pid;

void move_with_point(Vector2D point, double heading, PIDVariables xy_pid_vars, PIDVariables heading_pid_vars) {
    mwp_x_pid.set_PID_constants(xy_pid_vars);
    mwp_y_pid.set_PID_constants(xy_pid_vars);
    mwp_heading_pid.set_PID_constants(heading_pid_vars);

    mwp_x_pid.set_PID_variables(point.getHeadingBased(global_angle).x, 127, -127, 2);
    mwp_y_pid.set_PID_variables(point.getHeadingBased(global_angle).y, 127, -127, 2);
    mwp_heading_pid.set_PID_variables(heading, 127, -127, 1);

    double x_power = mwp_x_pid.output(GlobalPosition.getHeadingBased(global_angle).x);
    double y_power = mwp_y_pid.output(GlobalPosition.getHeadingBased(global_angle).y);
    double turn_power = mwp_heading_pid.output(imu.get_rotation());

    power_drive(x_power, y_power, turn_power);
}
