#include "Hole.h"

#include <algorithm>
#include <cmath>
#include <ostream>

namespace {
constexpr double EPS = 1e-6;

bool segmentEntryParameter(const golf::Vector2D &start, const golf::Vector2D &end,
                           const golf::Vector2D &minCorner, const golf::Vector2D &maxCorner,
                           double &entryT) {
    const double minX = std::min(minCorner.x(), maxCorner.x());
    const double maxX = std::max(minCorner.x(), maxCorner.x());
    const double minY = std::min(minCorner.y(), maxCorner.y());
    const double maxY = std::max(minCorner.y(), maxCorner.y());

    const golf::Vector2D displacement = end - start;
    const double dx = displacement.x();
    const double dy = displacement.y();

    double t0 = 0.0;
    double t1 = 1.0;

    const auto clip = [&](double p, double q) -> bool {
        if (std::abs(p) < EPS) {
            return q >= 0.0;
        }

        const double r = q / p;
        if (p < 0.0) {
            if (r > t1) {
                return false;
            }
            t0 = std::max(t0, r);
        } else {
            if (r < t0) {
                return false;
            }
            t1 = std::min(t1, r);
        }
        return true;
    };

    if (!clip(-dx, start.x() - minX) || !clip(dx, maxX - start.x()) || !clip(-dy, start.y() - minY) ||
        !clip(dy, maxY - start.y())) {
        return false;
    }

    if (t0 > t1 || t1 < 0.0 || t0 > 1.0) {
        return false;
    }

    entryT = std::clamp(t0, 0.0, 1.0);
    return true;
}
}

namespace golf {

Hole::Hole(std::string name, const Vector2D &teePosition, const Vector2D &cupPosition,
           std::vector<SandTrap> sandTraps, std::vector<WaterHazard> waterHazards, std::vector<Wall> walls)
    : name_{std::move(name)},
      teePosition_{teePosition},
      cupPosition_{cupPosition},
      sandTraps_{std::move(sandTraps)},
      waterHazards_{std::move(waterHazards)},
      walls_{std::move(walls)} {}

const std::string &Hole::name() const { return name_; }

const Vector2D &Hole::teePosition() const { return teePosition_; }

const Vector2D &Hole::cupPosition() const { return cupPosition_; }

double Hole::remainingDistance(const Vector2D &ballPosition) const {
    return (cupPosition_ - ballPosition).magnitude();
}

bool Hole::inHazard(const Vector2D &position) const {
    return isInSand(position) || isInWater(position);
}

Vector2D Hole::simulateBallRoll(const Vector2D &start, const Shot &shot, double friction,
                                bool &hazardTriggered) const {
    hazardTriggered = false;

    const Vector2D strokeStart = start;

    Vector2D displacement = shot.velocity().scaled(std::max(0.0, 1.0 - friction));
    const double initialDistance = displacement.magnitude();
    if (initialDistance <= EPS) {
        return start;
    }

    Vector2D direction = displacement.normalized();
    double remainingDistance = initialDistance;
    Vector2D currentPosition = start;

    constexpr int MAX_EVENTS = 16;
    for (int step = 0; step < MAX_EVENTS && remainingDistance > EPS; ++step) {
        const Vector2D segmentEnd = currentPosition + direction.scaled(remainingDistance);

        enum class Event { None, Sand, Water, Wall };
        struct Hit {
            Event type{Event::None};
            double t{1.1};
            const SandTrap *sand{nullptr};
            const WaterHazard *water{nullptr};
            const Wall *wall{nullptr};
        };

        Hit hit;

        for (const auto &trap : sandTraps_) {
            double entryT = 0.0;
            if (!segmentEntryParameter(currentPosition, segmentEnd, trap.minCorner(), trap.maxCorner(), entryT)) {
                continue;
            }
            if (entryT < hit.t - EPS) {
                hit.type = Event::Sand;
                hit.t = entryT;
                hit.sand = &trap;
                hit.water = nullptr;
                hit.wall = nullptr;
            }
        }

        for (const auto &hazard : waterHazards_) {
            double entryT = 0.0;
            if (!segmentEntryParameter(currentPosition, segmentEnd, hazard.minCorner(), hazard.maxCorner(), entryT)) {
                continue;
            }
            if (entryT < hit.t - EPS) {
                hit.type = Event::Water;
                hit.t = entryT;
                hit.sand = nullptr;
                hit.water = &hazard;
                hit.wall = nullptr;
            }
        }

        for (const auto &wall : walls_) {
            Vector2D intersection{0.0, 0.0};
            if (!wall.intersectsSegment(currentPosition, segmentEnd, intersection)) {
                continue;
            }

            const double travelRatio = (intersection - currentPosition).magnitude() / remainingDistance;
            if (travelRatio < hit.t - EPS) {
                hit.type = Event::Wall;
                hit.t = travelRatio;
                hit.sand = nullptr;
                hit.water = nullptr;
                hit.wall = &wall;
            }
        }

        if (hit.type == Event::None || hit.t > 1.0) {
            currentPosition = segmentEnd;
            remainingDistance = 0.0;
            break;
        }

        const double clampedT = std::clamp(hit.t, 0.0, 1.0);
        const double travelBeforeEvent = remainingDistance * clampedT;
        const Vector2D eventPosition = currentPosition + direction.scaled(travelBeforeEvent);
        remainingDistance -= travelBeforeEvent;
        currentPosition = eventPosition;

        if (remainingDistance < EPS) {
            break;
        }

        switch (hit.type) {
        case Event::Water:
            hazardTriggered = true;
            return strokeStart;
        case Event::Sand:
            if (hit.sand != nullptr) {
                hazardTriggered = true;
                remainingDistance *= hit.sand->slowdownFactor();
            }
            break;
        case Event::Wall:
            if (hit.wall != nullptr) {
                hazardTriggered = true;
                direction = hit.wall->reflect(direction);
            }
            break;
        case Event::None:
            break;
        }
    }

    if (remainingDistance > EPS) {
        currentPosition = currentPosition + direction.scaled(remainingDistance);
    }

    if ((cupPosition_ - currentPosition).magnitude() < 0.4) {
        currentPosition = cupPosition_;
    }

    return currentPosition;
}

bool Hole::isInSand(const Vector2D &position) const {
    for (const auto &trap : sandTraps_) {
        if (trap.contains(position)) {
            return true;
        }
    }
    return false;
}

bool Hole::isInWater(const Vector2D &position) const {
    for (const auto &hazard : waterHazards_) {
        if (hazard.contains(position)) {
            return true;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const Hole &hole) {
    os << "Hole '" << hole.name() << "'\n  Tee: " << hole.teePosition() << "\n  Cup: " << hole.cupPosition();
    os << "\n  Sand traps (" << hole.sandTraps_.size() << ")";
    for (const auto &trap : hole.sandTraps_) {
        os << "\n    - " << trap;
    }
    os << "\n  Water hazards (" << hole.waterHazards_.size() << ")";
    for (const auto &hazard : hole.waterHazards_) {
        os << "\n    - " << hazard;
    }
    os << "\n  Walls (" << hole.walls_.size() << ")";
    for (const auto &wall : hole.walls_) {
        os << "\n    - " << wall;
    }
    os << '\n';
    return os;
}

}