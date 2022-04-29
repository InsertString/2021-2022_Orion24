#include "main.h"



void drive_curve(quadratic_bezi_curve target, PIDConstants y_k, PIDConstants t_k, double velocity) {
    PID y_PID;
    PID t_PID;

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

        t_target = target.getFirstDerivative(bezi_input).getAngle();
        t_PID.set_target(t_target);

        power_drive_directional(y_PID.output(drive_position()), t_PID.output(imu.get_rotation()));
    }

    power_drive_directional(0,0);

}



void drive_straight(double target, PIDConstants y_k, PIDConstants t_k, double timeout, double accuracy) {

}



void turn(double target, PIDConstants t_k, double timeout, double accuracy) {

}



void wall_square(double timeout) {

}
