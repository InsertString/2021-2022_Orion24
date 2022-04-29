#ifndef _AUTO_FUNCTIONS_HPP_
#define _AUTO_FUNCTIONS_HPP_

void drive_curve(quadratic_bezi_curve target, PIDConstants y_k, PIDConstants t_k, double velocity);

void drive_straight(double target, PIDConstants y_k, PIDConstants t_k, double timeout, double accuracy);

void turn(double target, PIDConstants t_k, double timeout, double accuracy);

void wall_square(double timeout);

#endif