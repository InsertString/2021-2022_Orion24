#ifndef _AUTO_FUNCTIONS_HPP_
#define _AUTO_FUNCTIONS_HPP_

#include "Vector2D.hpp"
#include "PID.hpp"

#define FORWARD 1
#define BACKWARD -1

double in_to_cm(double in);

void move_to_point(int direction, Vector2D target, double accuracy, double max_power, double timeout, PIDVariables y_pid_vars, PIDVariables turn_pid_vars);
void move_to_point(int direction, Vector2D target, double max_power, double timeout, PIDVariables y_pid_vars, PIDVariables turn_pid_vars);

void rotate_to_heading(double heading, double accuracy, double max_power, double timeout, PIDVariables turn_pid_vars);
void rotate_to_heading(double heading, double max_power, double timeout, PIDVariables turn_pid_vars);

void home_arm();
void move_arm_auto(bool waiting, double target, double speed, double timeout);

void home_mogo();
void move_mogo_auto(double target, double speed, double timeout);

void set_wrist_position(bool send_up);

#endif