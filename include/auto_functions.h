#ifndef _AUTO_FUNCTIONS_H_
#define _AUTO_FUNCTIONS_H_

enum Auto_Function {
  COMPLETE,
  INCOMPLETE,
  FLAGGED
};

void move_with_point(Vector2D point, double heading, PIDVariables xy_pid_vars, PIDVariables heading_pid_vars);

Auto_Function follow_path(int &step, Vector2D path[], double path_angles[], const int path_size, int &path_index, double path_accuracy, int timeout);

#endif