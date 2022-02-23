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

Timer path_timeout;
Timer path_exit_timer;

Auto_Function follow_path(int &step, Vector2D path[], double path_angles[], const int path_size, int &path_index, double path_accuracy, int timeout) {
    switch (step) {
        case 0 : // setup
        path_index = 0;
        path_timeout.reset();
        step++;
        break;
        case 1 : // follow the path
        Vector2D path_error = path[path_index] - GlobalPosition;
        double dist_to_path_point = path_error.getLength();

        if (dist_to_path_point < path_accuracy && path_index < (path_size - 1)) {
            path_index++;
            break;
        }
        else if (dist_to_path_point < path_accuracy && path_index == (path_size - 1)) {
            step++;

            break;
        }

        move_with_point(path[path_index], path_angles[path_index], PIDVariables(6, 0.3, 10), PIDVariables(3, 0.1, 10));
        break;
        case 2 :
        step = 0;
        return COMPLETE;
        break;
    }

    return INCOMPLETE;
}
