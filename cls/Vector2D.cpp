#include "Vector2D.h"

#include <cmath>
#include <ostream>

namespace golf {

    Vector2D::Vector2D(double x, double y) : x_{x}, y_{y} {}

    Vector2D::Vector2D(const Vector2D &other) = default;

    Vector2D &Vector2D::operator=(const Vector2D &other) = default;

    Vector2D::~Vector2D() noexcept = default;

    double Vector2D::x() const { return x_; }

    double Vector2D::y() const { return y_; }

    double Vector2D::magnitude() const { return std::hypot(x_, y_); }

    Vector2D Vector2D::normalized() const {
        const double mag = magnitude();
        if (mag == 0.0) {
            return Vector2D{0.0, 0.0};
        }
        return Vector2D{x_ / mag, y_ / mag};
    }

    Vector2D Vector2D::scaled(double factor) const {
        return Vector2D{x_ * factor, y_ * factor};
    }

    double Vector2D::dot(const Vector2D &other) const {
        return x_ * other.x_ + y_ * other.y_;
    }

    double Vector2D::cross(const Vector2D &other) const {
        return x_ * other.y_ - y_ * other.x_;
    }

    Vector2D Vector2D::perpendicular() const {
        return Vector2D{-y_, x_};
    }

    Vector2D Vector2D::operator+(const Vector2D &other) const {
        return Vector2D{x_ + other.x_, y_ + other.y_};
    }

    Vector2D Vector2D::operator-(const Vector2D &other) const {
        return Vector2D{x_ - other.x_, y_ - other.y_};
    }

    std::ostream &operator<<(std::ostream &os, const Vector2D &vector) {
        os << "(" << vector.x() << ", " << vector.y() << ")";
        return os;
    }

}