#ifndef _AUTO_FUNCTIONS_H_
#define _AUTO_FUNCTIONS_H_

enum Auto_Function {
  COMPLETE,
  INCOMPLETE,
  FLAGGED
};

void move_with_point(Vector2D point, double heading, PIDVariables xy_pid_vars, PIDVariables heading_pid_vars);

Auto_Function follow_path(int &step, Vector2D path[], double path_angles[], const int path_size, int &path_index, double path_accuracy, int timeout);

void move_OBar(int &OBar_state);
void move_arm(int &arm_state);
void move_mogo(int &mogo_state, int &mogo_shifter_state, int &OBar_state, int &needle_state);
void set_claw(bool &claw_state);
void move_intake(int &intake_state);
bool intake_is_running();


#endif