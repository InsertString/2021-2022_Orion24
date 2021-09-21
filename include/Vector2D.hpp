#ifndef _VECTOR_CLAS_HPP_
#define _VECTOR_CLAS_HPP_

class Vector2D {
public:
  double x, y;
  Vector2D (){}
  Vector2D (double a, double b) : x(a), y(b) {}
  Vector2D operator + (const Vector2D&);
  Vector2D operator * (const double&);
  double getLength();
  double getAngle();
};

#endif
