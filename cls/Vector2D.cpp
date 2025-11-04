#include "Vector2D.h"
#include <cmath>
Vector2D::Vector2D(float x, float y): x(x), y(y) {}
float Vector2D::getX() const { return x; }
float Vector2D::getY() const { return y; }
float Vector2D::dist(const Vector2D& o) const {
    return sqrt((x - o.x)*(x - o.x) + (y - o.y)*(y - o.y));
}
Vector2D Vector2D::operator+(const Vector2D& o) const { return {o.x + x, y + o.y}; }
Vector2D Vector2D::operator*(float s) const { return {x * s, y * s}; }
ostream& operator<<(ostream& os, const Vector2D& v) {
    os << fixed << setprecision(2) << "(" << v.x << ", " << v.y << ")";
    return os;
}
