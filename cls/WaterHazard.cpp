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

    bool WaterHazard::intersectsSegment(const Vector2D &a, const Vector2D &b) const {
        // Obtinem colurile dreptunghiului
        const double minX = std::min(minCorner_.x(), maxCorner_.x());
        const double maxX = std::max(minCorner_.x(), maxCorner_.x());
        const double minY = std::min(minCorner_.y(), maxCorner_.y());
        const double maxY = std::max(minCorner_.y(), maxCorner_.y());

        //unul din capete e în apă
        if (contains(a) || contains(b)) {
            return true;
        }

        // Verificam dacă segmentul trece prin interiorul dreptunghiului
        // Folosim ecuația parametrizată a segmentului
        const Vector2D dir = b - a;

        double tMin = 0.0;
        double tMax = 1.0;

        if (std::abs(dir.x()) < 1e-8) {
            // Segment vertical
            if (a.x() < minX || a.x() > maxX)
                return false;
        } else {
            double tx1 = (minX - a.x()) / dir.x();
            double tx2 = (maxX - a.x()) / dir.x();
            tMin = std::max(tMin, std::min(tx1, tx2));
            tMax = std::min(tMax, std::max(tx1, tx2));
        }

        if (std::abs(dir.y()) < 1e-8) {
            // Segment orizontal
            if (a.y() < minY || a.y() > maxY)
                return false;
        } else {
            double ty1 = (minY - a.y()) / dir.y();
            double ty2 = (maxY - a.y()) / dir.y();
            tMin = std::max(tMin, std::min(ty1, ty2));
            tMax = std::min(tMax, std::max(ty1, ty2));
        }

        return tMax >= tMin && tMax >= 0.0 && tMin <= 1.0;
    }

    std::ostream &operator<<(std::ostream &os, const WaterHazard &hazard) {
        os << "WaterHazard(min=" << hazard.minCorner() << ", max=" << hazard.maxCorner() << ")";
        return os;
    }

}