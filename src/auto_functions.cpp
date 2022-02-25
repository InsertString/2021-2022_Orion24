#include "main.h"

PID chassis_y_pid;
PID chassis_turn_pid;

Timer exit_timer;
Timer timeout_timer;

void move_to_point(int &step, int direction, Vector2D target, double accuracy, double max_power, double timeout, PIDVariables y_pid_vars, PIDVariables turn_pid_vars) {
    Vector2D error = target - odom.getPosition();
    Vector2D local_current = odom.getPosition().getHeadingBased(odom.rad_angle());
    Vector2D local_target = target.getHeadingBased(odom.rad_angle());
    Vector2D local_error = local_target - local_current;
    bool exit_condition = false;

    switch (step) {
        case FUNC_SETUP :
        chassis_y_pid.set_PID_constants(y_pid_vars);
        chassis_turn_pid.set_PID_constants(turn_pid_vars);
        timeout_timer.reset();
        step = FUNC_BODY;
        break;
        case FUNC_BODY :
        printf("lTx:[%3.2f], lTy:[%3.2f], lCx:[%3.2f], lCy:[%3.2f], lEx:[%3.2f], lEy:[%3.2f]\n", local_target.x, local_target.y, local_current.x, local_current.y, local_error.x, local_error.y);
        chassis_y_pid.set_PID_variables(local_target.y, max_power, -max_power, 15);
        chassis_turn_pid.set_PID_variables(local_target.x, max_power, -max_power, 2);

        power_drive(chassis_y_pid.output(local_current.y), chassis_turn_pid.output(local_current.x) * direction,0,0);

        if (error.getLength() > accuracy) exit_timer.reset();

        if (exit_timer.delta_time() > 250) {
            exit_condition = true;
        }

        if (exit_condition == true || timeout_timer.delta_time() > timeout) {
            step = FUNC_COMPLETE;
            power_drive(0,0,0,0);
        }
        break;
        case FUNC_COMPLETE :
        power_drive(0,0,0,0);
        break;
    }
}


void move_to_point(int &step, int direction, Vector2D target, double max_power, double timeout, PIDVariables y_pid_vars, PIDVariables turn_pid_vars) {
    move_to_point(step, direction, target, 2, max_power, timeout, y_pid_vars, turn_pid_vars);
}


void rotate_to_heading(int &step, double heading, double accuracy, double max_power, double timeout, PIDVariables turn_pid_vars) {
    double error = heading - odom.getAngle();
    bool exit_condition = false;

    switch (step) {
        case FUNC_SETUP :
        chassis_turn_pid.set_PID_constants(turn_pid_vars);
        timeout_timer.reset();
        step = FUNC_BODY;
        break;
        case FUNC_BODY :
        chassis_turn_pid.set_PID_variables(heading, max_power, -max_power, 10);

        power_drive(0,chassis_turn_pid.output(odom.getAngle()),0,0);

        if (fabs(error) > accuracy) exit_timer.reset();
        else if (exit_timer.delta_time() > 250) exit_condition = true;

        if (exit_condition == true || timeout_timer.delta_time() > timeout) {
            step = FUNC_COMPLETE;
            power_drive(0,0,0,0);
        }
        break;
        case FUNC_COMPLETE :
        power_drive(0,0,0,0);
        break;
    }
}


void rotate_to_heading(int &step, double heading, double max_power, double timeout, PIDVariables turn_pid_vars) {
    rotate_to_heading(step, heading, 1.5, max_power, timeout, turn_pid_vars);
}
