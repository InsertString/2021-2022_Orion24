#ifndef _PID_HPP_
#define _PID_HPP_


class PIDConstants {
public:
  double kp;
  double ki;
  double kd;
  PIDConstants(double p, double i, double d);
};


// PID class //
/*

PID class is used to make PID objects which each have their own PID variables,
constants, and functions.

*/
class PID {
private:
  // PID variables
  double target;
  double error;
  double past_error;
  double integral;
  double derivative;
  double max;
  double min;
  double integral_limit;

  // PID constants

  double kp;
  double ki;
  double kd;

public:
  // constuctor for PID class
  PID();

  // sets PID targets and max motor power values with the
  void set_variables(double target_input, double max_value, double min_value, double integral_lim);
  void set_max_value(double max_value);
  void set_min_value(double min_value);
  void set_int_limit(double integral_lim);
  void set_target(double target);
  void set_error(double error);
  void set_past_error(double error);

  // sets the values of the PID constants
  void set_constants(double p, double i, double d);
  void set_constants(PIDConstants pid_var);

  // returns the output of your PID
  double output(double current);
  double output();
};


#endif
