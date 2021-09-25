#include "main.h"
#include "Vector2D.hpp"

using namespace std;

double Vector2D::getLength() {
  return sqrt(pow(x, 2) + pow(y, 2));
}

double Vector2D::getAngle() {
  if (x != 0) return tan(y / x);
  else if (y > 0) return (3.14159 / 2);
  else if (y < 0) return (3 * 3.14159 / 2);
  else return 0;
}

Vector2D Vector2D::operator+ (const Vector2D& param) {
  Vector2D temp;
  temp.x = x + param.x;
  temp.y = y + param.y;
  return temp;
}

Vector2D Vector2D::operator* (const double& param) {
  Vector2D temp;
  temp.x = x * param;
  temp.y = y * param;
  return temp;
}
