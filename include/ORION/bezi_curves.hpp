#ifndef _BEZI_CURVES_HPP_
#define _BEZI_CURVES_HPP_

#include "Vector2D.hpp"

class quadratic_bezi_curve {
private:
    #define BEZI_DEBUG_CURVE 1
    #define BEXI_DEBUG_FIRST_DERIVATIVE 2
    #define BEZI_DEBUG_SECOND_DERIVATIVE 3
    #define BEZI_DEBUG_VELOCITY 4
    #define BEZI_DEBUG_ANGULAR_VELOCITY 5
    int debug_state;
    Vector2D P[3];
public:
    void set_points(Vector2D P_0, Vector2D P_1, Vector2D P_2);
    void set_debug_state(int state);
    Vector2D getCurve(double t);
    Vector2D getFirstDerivative(double t);
    Vector2D getSecondDerivative(double t);
    double getVelocity(double t);
    double getAngularVelocity(double t);
};

#endif