#include "main.h"

void power_drive_differential(double left, double right) {

}

void power_drive_directional(double y, double turn) {

}

PID yPID;
PID tPID;

void path_to_point(Vector2D target) {
    PIDConstants yk(0, 0, 0);
    PIDConstants tk(0, 0, 0);

    Vector2D trans_target = target.getHeadingBased(odom.rad_angle());
    Vector2D trans_current = odom.getPosition().getHeadingBased(odom.rad_angle());

    yPID.set_constants(yk);
    yPID.set_int_limit(100);
    yPID.set_max_value(127);
    yPID.set_target(trans_target.y);
    
    tPID.set_constants(tk);
    tPID.set_int_limit(100);
    tPID.set_max_value(127);
    tPID.set_target(trans_target.x);

    power_drive_directional(yPID.output(trans_current.y), tPID.output(trans_current.x));
}
