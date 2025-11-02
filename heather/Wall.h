#pragma once

#include "Vector2D.h"

#include <iosfwd>

namespace golf {

    class Wall {
    public:
        Wall(const Vector2D &start, const Vector2D &end);

        const Vector2D &start() const;
        const Vector2D &end() const;
        Vector2D direction() const;
        Vector2D normal() const;
        bool intersectsSegment(const Vector2D &segmentStart, const Vector2D &segmentEnd, Vector2D &intersection) const;
        Vector2D reflect(const Vector2D &incomingDirection) const;

    private:
        Vector2D start_;
        Vector2D end_;
    };

    std::ostream &operator<<(std::ostream &os, const Wall &wall);

}