#pragma once

#include "Vector2D.h"

#include <iosfwd>

namespace golf {

    class WaterHazard {
    public:
        WaterHazard(const Vector2D &minCorner, const Vector2D &maxCorner);

        const Vector2D &minCorner() const;
        const Vector2D &maxCorner() const;
        Vector2D center() const;
        bool contains(const Vector2D &point) const;
        bool intersectsSegment(const Vector2D &a, const Vector2D &b) const;

    private:
        Vector2D minCorner_;
        Vector2D maxCorner_;
    };

    std::ostream &operator<<(std::ostream &os, const WaterHazard &hazard);

}