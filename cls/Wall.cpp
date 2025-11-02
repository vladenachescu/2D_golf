#include "Wall.h"

#include <cmath>
#include <ostream>

namespace golf {
    namespace {
        constexpr double EPSILON = 1e-6;
    }

    Wall::Wall(const Vector2D &start, const Vector2D &end) : start_{start}, end_{end} {}

    const Vector2D &Wall::start() const { return start_; }

    const Vector2D &Wall::end() const { return end_; }

    Vector2D Wall::direction() const { return end_ - start_; }

    Vector2D Wall::normal() const {
        Vector2D n = direction().perpendicular().normalized();
        return n;
    }

    bool Wall::intersectsSegment(const Vector2D &segmentStart, const Vector2D &segmentEnd, Vector2D &intersection) const {
        const Vector2D p = segmentStart;
        const Vector2D r = segmentEnd - segmentStart;
        const Vector2D q = start_;
        const Vector2D s = end_ - start_;

        const double rxs = r.cross(s);
        const double q_p_cross_r = (q - p).cross(r);

        if (std::abs(rxs) < EPSILON && std::abs(q_p_cross_r) < EPSILON) {
            // Colinear segments - treat as intersecting at the closest point.
            intersection = q;
            return true;
        }

        if (std::abs(rxs) < EPSILON) {
            return false; // Parallel and non-intersecting.
        }

        const double t = (q - p).cross(s) / rxs;
        const double u = (q - p).cross(r) / rxs;

        if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
            intersection = p + r.scaled(t);
            return true;
        }
        return false;
    }

    Vector2D Wall::reflect(const Vector2D &incomingDirection) const {
        Vector2D unitIncoming = incomingDirection.normalized();
        Vector2D n = normal();
        if (unitIncoming.dot(n) > 0.0) {
            n = n.scaled(-1.0);
        }
        const double projection = unitIncoming.dot(n);
        Vector2D reflected = unitIncoming - n.scaled(2.0 * projection);
        return reflected.normalized();
    }

    std::ostream &operator<<(std::ostream &os, const Wall &wall) {
        os << "Wall(start=" << wall.start() << ", end=" << wall.end() << ")";
        return os;
    }

} // namespace golf//
