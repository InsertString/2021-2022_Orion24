#include "main.h"

void power_drive_differential(double left, double right) {

}

void power_drive_directional(double y, double turn) {

}

void path_to_point(Vector2D target, Odom * odom, PID * y_pid, PID * turn_pid) {
    Vector2D error = target - odom->getPosition();
    double turn_current = error.getAngle() - odom->rad_angle();
    double y_current = error.getLength();

    y_pid->set_PID_target(0);
    turn_pid->set_PID_target(0);
    power_drive_directional(y_pid->output(y_current), turn_pid->output(turn_current));
}
