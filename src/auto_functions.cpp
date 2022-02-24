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
        MotorPriority[DRIVE] = 8;
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
        MotorPriority[DRIVE] = 1;
        return COMPLETE;
        break;
    }

    return INCOMPLETE;
}

bool intake_jammed = false;
Timer intake_timer;

bool intake_is_running() {
    if (fabs(Elevator.get_actual_velocity()) < 2) {
        return false;
    } 
    else {
        return true;
    }
}

void move_intake(int &intake_state) {
    if (intake_state == 1 && intake_jammed == false) {
        MotorPriority[INTAKE] = 2;
        Elevator = 127;

        if (intake_is_running() == false) {
            intake_jammed = true;
            intake_timer.reset();
        }
    }
    else if (intake_jammed == true) {
        if (intake_timer.delta_time() < 500) {
            if (intake_is_running() == true) {
                intake_jammed =  false;
            }
        }
        else if (intake_timer.delta_time() >= 500 && intake_timer.delta_time() < 750) {
            Elevator = -50;
        }
        else {
            intake_jammed = false;
        }
    }
    else if (intake_state == 2) {
        MotorPriority[INTAKE] = 2;
        Elevator = -127;
    }
    else {
        MotorPriority[INTAKE] = 0;
        Elevator = 0;
    }
}

void set_claw(bool &claw_state) {
    Claw.set_value(claw_state);
}


void set_needle(bool &needle_state) {
    Needle.set_value(needle_state);
}



void move_mogo(int &mogo_state, bool &mogo_shifter_state, int &OBar_state, bool &needle_state) {
    // state machine for mogo intake system
    switch (mogo_state) {
        case 0 :
            // power the mogo intake until the mogo_limit is triggered
            if (MogoLimit.get_value() == 0) {
                // only close the cap if the mogo is out of the way
                if (MogoLeft.get_position() < 100) {
                    mogo_shifter_state = false;
                }
                MogoLeft = -70;
                MogoRight = -70;
                MotorPriority[MOGO] = 2;
            }
            // stop the intake if it is triggered
            else {
                MogoLeft = 0;
                MogoRight = 0;
                mogo_shifter_state = false;
                MotorPriority[MOGO] = 0;
            }
        MotorPriority[ONE] = 1;
        break;
        case 1 :
        // move the mogo intake to the max out position
        // move with voltage if the mogo_limit is triggered
        if (MogoLimit.get_value() == 1) {
            MogoLeft = 70;
            MogoRight = 70;
        }
        // move with move_absolute if the mogo_limit is triggered
        else {
            MogoLeft.move_absolute(MOGO_MAX_POS, 100);
            MogoRight.move_absolute(MOGO_MAX_POS, 100);
        }
        // flatten the mogo intake so that it is easier to grab a mogo
        mogo_shifter_state = true;
        MotorPriority[MOGO] = 2;
        MotorPriority[ONE] = 1;			
        break;
        case 2 :
        OWrist.move_absolute(100, 50);
        if (MogoLeft.get_position() < (MOGO_MID_POS - 20)) {
            mogo_shifter_state = true;
            if (MogoLimit.get_value() == 1) {
                MogoLeft = 70;
                MogoRight = 70;
            }
            else {
                MogoLeft.move_absolute(MOGO_MID_POS, 100);
                MogoRight.move_absolute(MOGO_MID_POS, 100);
            }
        }
        else {
            mogo_state++;
            OBar_state = 1;
        }
        MotorPriority[MOGO] = 2;
        MotorPriority[ONE] = 1;
        break;
        case 3 :
        OWrist.move_absolute(100, 50);
        MogoLeft.move_absolute(MOGO_MID_POS, 100);
        MogoRight.move_absolute(MOGO_MID_POS, 100);
        
        if (OBarAngle() < -45) {
            mogo_shifter_state = true;
        }

        if (OBarAngle() < -110) {
            mogo_state++;
        }
        MotorPriority[MOGO] = 1;
        MotorPriority[ONE] = 1;
        break;
        case 4 :
        OWrist.move_absolute(-50, 200);
        MogoLeft.move_absolute(MOGO_MID_POS, 100);
        MogoRight.move_absolute(MOGO_MID_POS, 100);
        needle_state = true;
        mogo_shifter_state = false;
        MotorPriority[MOGO] = 1;
        MotorPriority[ONE] = 1;
        break;
        case 5 :
        needle_state = false;
        OWrist.move_absolute(200, 50);
        OBar_state = 0;
        mogo_shifter_state = true;

        if (OBarAngle() > -60)
            mogo_state = 0;
        break;
    }
}


void move_arm(int &arm_state) {
    switch (arm_state) {
        case ARM_MIN :
        if (ArmLimit.get_value() == 0) {
            ArmLeft.move_absolute(ARM_MIN_POS, 200);
            ArmRight.move_absolute (ARM_MIN_POS, 200);
            MotorPriority[ARM] = 2;
        }
        else {
            ArmLeft = 0;
            ArmRight = 0;
            MotorPriority[ARM] = 0;
        }
        break;
        case ARM_HOVER :
        if (ArmLimit.get_value() == 1) {
            ArmLeft = 70;
            ArmRight = 70;
        }
        else {
            ArmLeft.move_absolute(ARM_HOVER_POS, 100);
            ArmRight.move_absolute(ARM_HOVER_POS, 100);
        }
        MotorPriority[ARM] = 1;
        break;
        case ARM_STACK :
        ArmLeft.move_absolute(ARM_STACK_POS, 100);
        ArmRight.move_absolute(ARM_STACK_POS, 100);
        MotorPriority[ARM] = 2;
        break;
        case ARM_MAX :
        ArmLeft.move_absolute(ARM_MAX_POS, 200);
        ArmRight.move_absolute(ARM_MAX_POS, 200);
        MotorPriority[ARM] = 2;
        break;
        case ARM_DOCK_POS :
        ArmLeft.move_absolute(ARM_DOCK_POS, 200);
        ArmRight.move_absolute(ARM_DOCK_POS, 200);
        MotorPriority[ARM] = 2;
        break;
    }
}


void move_OBar(int &OBar_state) {
    switch (OBar_state) {
        case 0 :
        OBarMoveToPosition(0);
        break;
        case 1 :
        OBarMoveToPosition(-120);
        break;
        case 2 :
        OBarMoveToPosition(-4);
        OWrist.move_absolute(-10, 50);
        break;
        case 3 :
        OBarMoveToPosition(0);
        break;
    }
}