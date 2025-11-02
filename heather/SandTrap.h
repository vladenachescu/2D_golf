#pragma once

#include "Vector2D.h"

#include <iosfwd>

namespace golf {

    class SandTrap {
    public:
        SandTrap(const Vector2D &minCorner, const Vector2D &maxCorner, double slowdownFactor);

        const Vector2D &minCorner() const;
        const Vector2D &maxCorner() const;
        Vector2D center() const;
        double slowdownFactor() const;
        bool contains(const Vector2D &point) const;

    private:
        Vector2D minCorner_;
        Vector2D maxCorner_;
        double slowdownFactor_;
    };

    std::ostream &operator<<(std::ostream &os, const SandTrap &trap);

}