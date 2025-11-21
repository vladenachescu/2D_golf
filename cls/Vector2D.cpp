#include "Vector2D.h"
#include <cmath>
Vector2D::Vector2D(float x, float y): x(x), y(y) {}
float Vector2D::getX() const { return x; }
float Vector2D::getY() const { return y; }
float Vector2D::dist(const Vector2D& o) const {
    return std::sqrt((x - o.x)*(x - o.x) + (y - o.y)*(y - o.y));
}
float Vector2D::lungime() const {
    return std::sqrt(x * x + y * y);
}
Vector2D Vector2D::normalizat() const {
    float len = lungime();
    if (len < 1e-6f) return Vector2D(0,0);
    return Vector2D(x / len, y / len);
}
Vector2D Vector2D::operator+(const Vector2D& o) const {
    return Vector2D(x + o.x, y + o.y);
}

Vector2D Vector2D::operator*(float s) const {
    return Vector2D(x * s, y * s);
}

Vector2D Vector2D::operator/(float s) const {
    if (std::fabs(s) < 1e-6f) return Vector2D(0,0);
    return Vector2D(x / s, y / s);
}

Vector2D Vector2D::operator-(const Vector2D& o) const {
    return Vector2D(x - o.x, y - o.y);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << std::fixed << std::setprecision(2) << "(" << v.x << ", " << v.y << ")";
    return os;
}
