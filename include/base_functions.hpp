#ifndef _BASE_FUNCTIONS_HPP_
#define _BASE_FUNCTIONS_HPP_

void power_drive(double y, double turn, double left, double right);
void power_arm(double power);
void reset_arm_position();
void move_arm_to_position(double target, double speed);
void power_intake(double power);
void power_mogo(double power);
void reset_mogo_position();
void move_mogo_to_position(double target, double speed);
void move_wrist(double power);

#define DEBUG_BASE_DRIVE 1
#define DEBUG_BASE_ARM_POS 2
#define DEBUG_BASE_INTAKE 3
#define DEBUG_BASE_MOGO_POS 4


void debug_base_systems();

#endif