#pragma once

#include <iosfwd>

namespace golf {

    class Vector2D {
    public:
        Vector2D(double x, double y);
        Vector2D(const Vector2D &other);
        Vector2D &operator=(const Vector2D &other);
        ~Vector2D() noexcept;

        double x() const;
        double y() const;
        double magnitude() const;
        Vector2D normalized() const;
        Vector2D scaled(double factor) const;
        double dot(const Vector2D &other) const;
        double cross(const Vector2D &other) const;
        Vector2D perpendicular() const;
        Vector2D operator+(const Vector2D &other) const;
        Vector2D operator-(const Vector2D &other) const;

    private:
        double x_;
        double y_;
    };

    std::ostream &operator<<(std::ostream &os, const Vector2D &vector);

}