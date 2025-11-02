#include "WaterHazard.h"

#include <algorithm>
#include <ostream>

namespace golf {

    WaterHazard::WaterHazard(const Vector2D &minCorner, const Vector2D &maxCorner)
        : minCorner_{minCorner}, maxCorner_{maxCorner} {}

    const Vector2D &WaterHazard::minCorner() const { return minCorner_; }

    const Vector2D &WaterHazard::maxCorner() const { return maxCorner_; }

    Vector2D WaterHazard::center() const {
        return Vector2D{(minCorner_.x() + maxCorner_.x()) / 2.0, (minCorner_.y() + maxCorner_.y()) / 2.0};
    }

    bool WaterHazard::contains(const Vector2D &point) const {
        const double minX = std::min(minCorner_.x(), maxCorner_.x());
        const double maxX = std::max(minCorner_.x(), maxCorner_.x());
        const double minY = std::min(minCorner_.y(), maxCorner_.y());
        const double maxY = std::max(minCorner_.y(), maxCorner_.y());
        return point.x() >= minX && point.x() <= maxX && point.y() >= minY && point.y() <= maxY;
    }

    std::ostream &operator<<(std::ostream &os, const WaterHazard &hazard) {
        os << "WaterHazard(min=" << hazard.minCorner() << ", max=" << hazard.maxCorner() << ")";
        return os;
    }

}