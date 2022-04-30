#ifndef _AUTO_FUNCTIONS_HPP_
#define _AUTO_FUNCTIONS_HPP_

void drive_curve(quadratic_bezi_curve target, PIDConstants y_k, PIDConstants t_k, double velocity);

void drive_straight(double target, PIDConstants y_k, PIDConstants t_k, double max, double timeout, double accuracy);

void turn(double target, PIDConstants t_k, double timeout, double accuracy);

void wall_square(double timeout);

void rings_open();
void rings_close();

void deploy_stage_1();

void arm_down();
void arm_up();

void store_goal();
void idle_conv(double power);

#endif