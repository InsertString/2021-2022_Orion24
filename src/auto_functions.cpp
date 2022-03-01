#include "main.h"

PID chassis_y_pid;
PID chassis_turn_pid;

Timer exit_timer;
Timer timeout_timer;

void move_to_point(int direction, Vector2D target, double accuracy, double max_power, double timeout, PIDVariables y_pid_vars, PIDVariables turn_pid_vars) {
    Vector2D error = target - odom.getPosition();
    Vector2D local_current = odom.getPosition().getHeadingBased(odom.rad_angle());
    Vector2D local_target = target.getHeadingBased(odom.rad_angle());
    Vector2D local_error = local_target - local_current;
    bool exit_condition = false;
    chassis_y_pid.set_PID_constants(y_pid_vars);
    chassis_turn_pid.set_PID_constants(turn_pid_vars);
    timeout_timer.reset();

    while (true) {
        error = target - odom.getPosition();
        local_current = odom.getPosition().getHeadingBased(odom.rad_angle());
        local_target = target.getHeadingBased(odom.rad_angle());
        local_error = local_target - local_current;
        //printf("lTx:[%3.2f], lTy:[%3.2f], lCx:[%3.2f], lCy:[%3.2f], lEx:[%3.2f], lEy:[%3.2f]\n", local_target.x, local_target.y, local_current.x, local_current.y, local_error.x, local_error.y);
        chassis_y_pid.set_PID_variables(local_target.y, max_power, -max_power, 15);
        chassis_turn_pid.set_PID_variables(local_target.x, max_power, -max_power, 2);

        power_drive(chassis_y_pid.output(local_current.y), chassis_turn_pid.output(local_current.x) * direction,0,0);

        if (error.getLength() > accuracy) exit_timer.reset();

        if (exit_timer.delta_time() > 250) {
            exit_condition = true;
        }

        if (exit_condition == true || timeout_timer.delta_time() > timeout) {
            power_drive(0,0,0,0);
            break;
        }
    }
    power_drive(0,0,0,0);
}


void move_to_point(int direction, Vector2D target, double max_power, double timeout, PIDVariables y_pid_vars, PIDVariables turn_pid_vars) {
    move_to_point(direction, target, 3, max_power, timeout, y_pid_vars, turn_pid_vars);
}


void rotate_to_heading(double heading, double accuracy, double max_power, double timeout, PIDVariables turn_pid_vars) {
    double error = heading - odom.getAngle();
    bool exit_condition = false;
    timeout_timer.reset();
    chassis_turn_pid.set_PID_constants(turn_pid_vars);
    chassis_turn_pid.set_PID_variables(heading, max_power, -max_power, 10);

    while (true) {
        error = heading - odom.getAngle();
        power_drive(0,chassis_turn_pid.output(odom.getAngle()),0,0);

        if (fabs(error) > accuracy) exit_timer.reset();
        else if (exit_timer.delta_time() > 250) exit_condition = true;

        if (exit_condition == true || timeout_timer.delta_time() > timeout) {
            power_drive(0,0,0,0);
            break;
        }
    }
    power_drive(0,0,0,0);
}


void rotate_to_heading(double heading, double max_power, double timeout, PIDVariables turn_pid_vars) {
    rotate_to_heading(heading, 0.5, max_power, timeout, turn_pid_vars);
}


void home_arm() {
    // touch arm endstop fast
    while (ArmEndstop.get_value() != ARM_ENDSTOP_TRIGGERED) {
        power_arm(-100);
    }
    reset_arm_position();
    power_arm(0);
    // retouch slow for accuracy
    while (ArmEndstop.get_value() != ARM_ENDSTOP_TRIGGERED) {
        power_arm(-50);
    }
    reset_arm_position();
    power_arm(0);
}


void move_arm_auto(bool waiting, double target, double speed, double timeout) {
    double error = target - arm_position();
    Timer timeout_t;

    move_arm_to_position(target, speed);
    if (waiting) {
        while (fabs(error) < 10 || timeout_t.delta_time() > timeout) {
        // wait for the arm to get to position before continuing the auto
        }
    }
    else {
        delay(100);
    }
}


void home_mogo() {
    // touch mogo endstop fast
    while (MogoEndstop.get_value() != MOGO_ENDSTOP_TRIGGERED) {
        power_mogo(-100);
    }
    reset_mogo_position();
    power_mogo(0);
    // retouch slow for accuracy
    while (MogoEndstop.get_value() != MOGO_ENDSTOP_TRIGGERED) {
        power_mogo(-50);
    }
    reset_mogo_position();
    power_mogo(0);
}


void move_mogo_auto(bool waiting, double target, double speed, double timeout) {
    double error = target - mogo_position();
    Timer timeout_t;

    move_mogo_to_position(target, speed);
    if (waiting) {
        while (fabs(error) < 10 || timeout_t.delta_time() > timeout) {
        // wait for the arm to get to position before continuing the auto
        }
    }
    else {
        delay(100);
    }
}


void set_wrist_position(bool send_up) {
    double main_power = (send_up) ? 90 : -90;
    double hold_power = (send_up) ? 3 : -3;

    power_wrist(main_power);
    delay(500);
    power_wrist(hold_power);
}
