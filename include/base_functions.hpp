#ifndef _BASE_FUNCTIONS_HPP_
#define _BASE_FUNCTIONS_HPP_

void power_drive_differential(double left, double right);

void power_drive_directional(double y, double turn);

void tare_drive_motors();

double drive_position();

double convert_drive_deg_to_cm(double input);
double in_to_cm(double in);

void path_to_point(Vector2D target);

void power_conveyor(double left, double right);

void stabalize(PIDConstants k, double max, double int_limit);

void engage_drive_brake();
void disengage_drive_brake();

#endif