#include "SandTrap.h"

#include <algorithm>
#include <ostream>

namespace golf {

    SandTrap::SandTrap(const Vector2D &minCorner, const Vector2D &maxCorner, double slowdownFactor)
        : minCorner_{minCorner}, maxCorner_{maxCorner}, slowdownFactor_{slowdownFactor} {}

    const Vector2D &SandTrap::minCorner() const { return minCorner_; }

    const Vector2D &SandTrap::maxCorner() const { return maxCorner_; }

    Vector2D SandTrap::center() const { return Vector2D{(minCorner_.x() + maxCorner_.x()) / 2.0,
                                                        (minCorner_.y() + maxCorner_.y()) / 2.0}; }

    double SandTrap::slowdownFactor() const { return slowdownFactor_; }

    bool SandTrap::contains(const Vector2D &point) const {
        const double minX = std::min(minCorner_.x(), maxCorner_.x());
        const double maxX = std::max(minCorner_.x(), maxCorner_.x());
        const double minY = std::min(minCorner_.y(), maxCorner_.y());
        const double maxY = std::max(minCorner_.y(), maxCorner_.y());
        return point.x() >= minX && point.x() <= maxX && point.y() >= minY && point.y() <= maxY;
    }

    std::ostream &operator<<(std::ostream &os, const SandTrap &trap) {
        os << "SandTrap(min=" << trap.minCorner() << ", max=" << trap.maxCorner()
           << ", slowdown=" << trap.slowdownFactor() << ")";
        return os;
    }

}