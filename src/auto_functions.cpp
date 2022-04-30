#include "main.h"



void drive_curve(quadratic_bezi_curve target, PIDConstants y_k, PIDConstants t_k, double velocity) {
    PID y_PID;
    PID t_PID;

    tare_drive_motors();
    imu.tare();

    y_PID.set_constants(y_k);
    y_PID.set_variables(0, 127, -127, 100);

    t_PID.set_constants(t_k);
    t_PID.set_variables(0, 127, -127, 100);

    // FIND LENGTH OF CURVE //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    double total_dist = 0;
    double segment_dist = 0;
    double current_dist = 0;
    double past_dist = 0;
    double counter = 0;
    double bezi_input = 0;

    // sum up segments of bezi curve to find approximate length
    while (counter < 200) {
        bezi_input = counter / 200.0;
        past_dist = current_dist;
        current_dist = target.getCurve(bezi_input).getLength();
        segment_dist = current_dist - past_dist;
        total_dist += segment_dist;
        counter++;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    // FIND RUNTIME FROM DISTANCE //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    double runtime_sec = total_dist / velocity;
    double runtime_ms = runtime_sec * 1000;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    Timer runtime_timer;
    Timer exit_timer;

    double y_target = 0;
    double t_target = 0;

    while (runtime_timer.delta_time() < runtime_ms) {
        bezi_input = runtime_timer.delta_time() / runtime_ms;

        y_target = target.getCurve(bezi_input).getLength();
        y_PID.set_target(y_target);

        t_target = target.getFirstDerivative(bezi_input).getAngle() * 180.0 / 3.1415;
        t_PID.set_target(t_target);

        power_drive_directional(y_PID.output(drive_position()), t_PID.output(imu.get_rotation()));
        delay(20);
    }

    power_drive_directional(0,0);

}



void drive_straight(double target, PIDConstants y_k, PIDConstants t_k, double max, double timeout, double accuracy) {
    PID y_PID;
    PID t_PID;
    Timer timeout_timer;
    Timer exit_timer;
    bool exit = false;
    double error = target - drive_position();

    y_PID.set_constants(y_k);
    y_PID.set_variables(target, max, -max, 2);

    t_PID.set_constants(t_k);
    t_PID.set_variables(0, 100, -100, 10);

    imu.tare();
    tare_drive_motors();

    while (exit == false) {
        error = target - drive_position();
        if (fabs(error) < accuracy) {
            if (exit_timer.delta_time() > 300) exit = true;
        }
        else exit_timer.reset();

        if (timeout_timer.delta_time() > timeout) exit = true;
        
        power_drive_directional(y_PID.output(drive_position()), t_PID.output(imu.get_rotation()));
    }

    power_drive_directional(0,0);
}



void turn(double target, PIDConstants t_k, double timeout, double accuracy) {
    PID t_PID;
    Timer timeout_timer;
    Timer exit_timer;
    bool exit = false;
    double error = target - imu.get_rotation();

    t_PID.set_constants(t_k);
    t_PID.set_variables(target, 100, -100, 3);

    imu.tare();

    while (exit == false) {
        error = target - imu.get_rotation();
        if (fabs(error) < accuracy) {
            if (exit_timer.delta_time() > 300) exit = true;
        }
        else exit_timer.reset();

        if (timeout_timer.delta_time() > timeout) exit = true;
        
        power_drive_directional(0, t_PID.output(imu.get_rotation()));
    }

    power_drive_directional(0,0);
}



void wall_square(double timeout) {
    power_drive_differential(-60, -60);
    delay(timeout);
    power_drive_differential(0, 0);
    delay(250);
    power_drive_differential(20, 20);
    delay(250);
    power_drive_differential(0, 0);
    delay(250);
    power_drive_differential(-20, -20);
    delay(timeout);
    power_drive_differential(0, 0);
}


void rings_open() {
    HighRings.set_value(true);
}

void rings_close() {
    HighRings.set_value(false);
}

void deploy_stage_1() {
    Clamp.set_value(true);
    delay(1000);
    Clamp.set_value(false);
}

void arm_down() {
    Arm.set_value(true);
}

void arm_up() {
    Arm.set_value(false);
}

void store_goal() {
    power_conveyor(-127, -127);
    delay(1000);
    power_conveyor(0, 0);
    delay(500);
    idle_conv(-40);
}

void idle_conv(double power) {
    power_conveyor(power, power);
}
