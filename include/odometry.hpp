#ifndef _ODOMETRY_HPP_
#define _ODOMETRY_HPP_

double DistCM(int a);
void CalculatePosition();
void odomDebug();
double global_angle_d();
double gyro_value();

extern double past_angle;
extern double new_angle;
extern double average_angle;
extern double delta_angle;
extern double past_global_angle;
extern double global_angle;

extern double delta_enc[3];
extern double past_enc[3];

#endif
