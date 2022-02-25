#include "main.h"

Vector2D Odom::getPosition() {
  return global_position;
}

Vector2D Odom::getVelocity() {
  return velocity;
}

Vector2D Odom::getAcceleration() {
  return acceleration;
}

double Odom::getAngle() {
  double val = (int)((imu.get_rotation() + initial_angle) * 100);
  return val / 100.0f;
}

double Odom::rad_angle() {
  return getAngle() / 180 * 3.1415;
}

void Odom::configure(double x_e_dist, double x_wheel_c, double y_e_dist, double y_wheel_c, double delay) {
  x_encoder_dist = x_e_dist;
  x_wheel_circumfrance = x_wheel_c;
  y_encoder_dist = y_e_dist;
  y_wheel_circumfrance = y_wheel_c;
  tracking_delay = delay;
  velocity = Vector2D(0,0);
  acceleration = Vector2D(0,0);
  global_position = Vector2D(0,0);
  global_offset = Vector2D(0,0);
}

void Odom::configure_starting(Vector2D init_pos, double init_angle) {
  initial_position = init_pos;
  initial_angle = init_angle;
}

void Odom::collect_data(int debug) {
  // store current values before they update
  past_x_encoder = XEncoder.get_value();
  past_y_encoder = YEncoder.get_value();
  past_angle = rad_angle();

  // wait for update
  delay(tracking_delay);

  // calculate the sensor deltas
  delta_x_encoder = XEncoder.get_value() - past_x_encoder;
  delta_y_encoder = YEncoder.get_value() - past_y_encoder;
  delta_angle = rad_angle() - past_angle;

  // convert encoder values to CM
  delta_x_encoder = delta_x_encoder / 360 * x_wheel_circumfrance;
  delta_y_encoder = delta_y_encoder / 360 * y_wheel_circumfrance;

  // print stuff for debugging
  if (debug == ODOM_DEBUG_ENCODER_RAW) {
    printf("\rX Enc = [%4.0d] Y Enc = [%4.0d]", XEncoder.get_value(), YEncoder.get_value());
    fflush(stdout);
  }
  
  if (debug == ODOM_DEBUG_ENCODER_CM) {
    printf("\rX Enc = [%3.2f cm] Y Enc = [%3.2f cm]", (double(XEncoder.get_value()) / 360.0f * x_wheel_circumfrance), (double(YEncoder.get_value()) / 360.0f * y_wheel_circumfrance));
    fflush(stdout);
  }
}

void Odom::calculate_position(int debug) {
  // collect data first
  collect_data(debug);

  // calculate local offset
  local_offset.x = delta_x_encoder + (delta_angle * x_encoder_dist);
  local_offset.y = delta_y_encoder + (delta_angle * y_encoder_dist);

  // calculate global offset
  global_offset.x = (local_offset.y * sin(rad_angle())) + (local_offset.x * cos(-rad_angle()));
  global_offset.y = (local_offset.y * cos(rad_angle())) + (local_offset.x * sin(-rad_angle()));

  // apply global offset to the robot
  global_position = global_position + global_offset;

  // calculate velocity based on offset
  double one_over_delay_in_seconds = 1 / (tracking_delay / 1000);
  velocity = global_offset * one_over_delay_in_seconds;

  if (debug == ODOM_DEBUG_GLOBAL_POSITION) {
    printf("\rUpdate Time: [%3.0fms] X: [%3.2f] Y: [%3.2f] Angle: [%3.2f]", tracking_delay, global_position.x, global_position.y, getAngle());
    fflush(stdout);
  }

  if (debug == ODOM_DEBUG_LOCAL_OFFSET) {
    printf("\rUpdate Time: [%3.0fms] X: [%3.2f] Y: [%3.2f]", tracking_delay, local_offset.x, local_offset.y);
    fflush(stdout);
  }

  if (debug == ODOM_DEBUG_VELOCITY) {
    printf("\rUpdate Time: [%3.0fms] X: [%3.2f] Y: [%3.2f]", tracking_delay, velocity.x, velocity.y);
    fflush(stdout);
  }
}
