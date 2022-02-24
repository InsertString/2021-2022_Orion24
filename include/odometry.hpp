#ifndef _ODOMETRY_HPP_
#define _ODOMETRY_HPP_

// Object for 2wheel Odometry
class Odom {
private:
    // angle variables
    double rad_angle();
    double delta_angle;
    double past_angle;
    // encoder variables
    double delta_y_encoder;
    double past_y_encoder;
    double delta_x_encoder;
    double past_x_encoder;
    // vectors
    Vector2D local_offset;
    Vector2D global_offset;
    Vector2D global_position;
    Vector2D velocity;
    Vector2D acceleration;
    // robot parameters
    double y_encoder_dist;
    double x_encoder_dist;
    double y_wheel_circumfrance;
    double x_wheel_circumfrance;
    Vector2D initial_position;
    double initial_angle;
    double tracking_delay;
public:
    Vector2D getPosition();
    Vector2D getVelocity();
    Vector2D getAcceleration();
    double getAngle();

    void configure(double x_e_dist, double x_wheel_c, double y_e_dist, double y_wheel_c, double delay);
    void configure_starting(Vector2D init_pos, double init_angle);

    void collect_data(int debug_flag);
    void calculate_position(int debug_flag);
};

void odom_task(void* param);

#endif
