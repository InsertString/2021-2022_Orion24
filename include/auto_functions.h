#ifndef _AUTO_FUNCTIONS_H_
#define _AUTO_FUNCTIONS_H_

enum Auto_Function {
  COMPLETE,
  INCOMPLETE,
  FLAGGED
};

void move_with_point(Vector2D point, double heading, PIDVariables xy_pid_vars, PIDVariables heading_pid_vars);

#endif