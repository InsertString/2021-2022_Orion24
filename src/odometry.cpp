#include "main.h"
#include "odometry.hpp"

using namespace std;

// (90 encoderticks / 1 rot) * (1 rot / circumfrance)

// wheels are 200mm circumfrance
double DistCM(int a) {
  if (a == 0) return (RightEncoder.get_value() / 360.0 * 20.0);
  else return (BackEncoder.get_value() / 360.0 * 20.0);
}


double imu_value() {
  int factor_of_360 = 0;
  factor_of_360 = imu.get_rotation() / 360;
  return imu.get_rotation();// - (360 * factor_of_360);
}

void odomDebug() {
  printf("%4.0f, %4.0f, %4.0f\n", GlobalPosition.x, GlobalPosition.y, imu_value());
}

double sideR = 17.5;
double sideB = 16.75;

Vector2D pastGlobalPosition(0,0);
Vector2D GlobalPosition(0,0);
Vector2D localOffset(0,0);
Vector2D globalOffset(0,0);

Vector2D local_x(0,0);
Vector2D local_y(0,0);

double delta_enc[2] = {};
double past_enc[2] = {};

double past_angle = 0;
double new_angle = 0;
double average_angle = 0;
double delta_angle = 0;
double past_global_angle = 0;
double global_angle = 0;

double global_angle_d() {
  return global_angle * 180 / 3.1415;
}

#define RIGHT 0
#define BACK 1


void CalculatePosition() {
  // only run the position calculations when the imu is not initializing
  // other wise the positions will error out at infinity
  if (imu.is_calibrating() == false) {

    // setup for next reset
    past_angle = (imu_value() / 180 * 3.1415);
    for (int i = 0; i < 2; i++) {
      past_enc[i] = DistCM(i);
    }
    pastGlobalPosition = GlobalPosition;

    // delay for values of encoders to change
    delay(20);

    // calulate change in encoder values
    for (int i = 0; i < 2; i++) {
      delta_enc[i] = DistCM(i) - past_enc[i];
    }

    // calculate change in angle
    new_angle = (imu_value() / 180 * 3.1415);
    global_angle = (imu_value() / 180 * 3.1415);

    delta_angle = new_angle - past_angle;

    // calculate localOffset

    // local offset x term is based on the change in the back encoder and the arc formed by the
    // imu value with identical radius to the back encoder, this simulates having a front encoder
    localOffset.x = delta_enc[BACK] + (delta_angle * sideB);

    // local offset y term is based on triangles
    localOffset.y = delta_enc[RIGHT] + (delta_angle * sideR);

    // in order to convert the local offset vector to a global offset vector, you need
    // to turn each component of the local position vector into global position vector rotated
    // by the global angle.
    // then combine the two new global vectors to get a global offset vector.

    local_y.x = localOffset.y * sin(global_angle);
    local_y.y = localOffset.y * cos(global_angle);

    local_x.x = localOffset.x * cos(-global_angle);
    local_x.y = localOffset.x * sin(-global_angle);

    globalOffset = local_y + local_x;

    // calculate global position based on the change from the prervious global position
    GlobalPosition = pastGlobalPosition + globalOffset;
  }
  else {
    GlobalPosition.x = 0;
    GlobalPosition.y = 0;
  }
}
