#include "main.h"

void quadratic_bezi_curve::set_points(Vector2D P_0, Vector2D P_1, Vector2D P_2) {
    P[0] = P_0;
    P[1] = P_1;
    P[2] = P_2;
}

void quadratic_bezi_curve::set_debug_state(int state) {
    debug_state = state;
}

Vector2D quadratic_bezi_curve::getCurve(double t) {
    Vector2D term_1;
    Vector2D term_2;
    Vector2D term_3;

    term_1.x = (pow((1 - t),2)) * P[0].x;
    term_1.y = (pow((1 - t),2)) * P[0].y;

    term_2.x = ((2 * t) * (1 - t)) * P[1].x;
    term_2.y = ((2 * t) * (1 - t)) * P[1].y;

    term_3.x = (pow(t, 2)) * P[2].x;
    term_3.y = (pow(t, 2)) * P[2].y;

    return term_1 + term_2 + term_3;
}

Vector2D quadratic_bezi_curve::getFirstDerivative(double t) {
    Vector2D term_1;
    Vector2D term_2;

    term_1.x = 2 * (1 - t) * (P[1].x - P[0].x);
    term_1.y = 2 * (1 - t) * (P[1].y - P[0].y);

    term_2.x = (2 * t) * (P[2].x - P[1].x);
    term_2.y = (2 * t) * (P[2].y - P[1].y);

    return term_1 + term_2;
}

Vector2D quadratic_bezi_curve::getSecondDerivative(double t) {
    Vector2D term_1;

    term_1.x = 2 * (P[2].x - (2 * P[1].x) + P[0].x);
    term_1.y = 2 * (P[2].y - (2 * P[1].y) + P[0].y);

    return term_1;
}

double quadratic_bezi_curve::getVelocity(double t) {
    return this->getFirstDerivative(t).getLength();
}

double quadratic_bezi_curve::getAngularVelocity(double t) {
    double numerator;
    double denominator;
    double term_1;
    double term_2;

    term_1 = (this->getFirstDerivative(t).x) * (this->getSecondDerivative(t).y);
    term_2 = (this->getFirstDerivative(t).y) * (this->getSecondDerivative(t).x);
    numerator = term_1 - term_2;
    denominator = pow(this->getFirstDerivative(t).x, 2) + pow(this->getFirstDerivative(t).y, 2);

    return numerator / denominator;
}

