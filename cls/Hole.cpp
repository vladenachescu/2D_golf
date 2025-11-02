#include "Hole.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <ostream>
#include <utility>

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
    const Vector2D desiredMove = shot.velocity();
    Vector2D displacement = desiredMove.scaled(std::max(0.0, 1.0 - friction));
    Vector2D destination = start + displacement;

    hazardTriggered = false;

    if (bounceAgainstWall(start, destination, displacement)) {
        hazardTriggered = true;
    }

    for (const auto &trap : sandTraps_) {
        if (trap.contains(destination)) {
            hazardTriggered = true;
            displacement = displacement.scaled(trap.slowdownFactor());
            destination = start + displacement;
            break;
        }
    }

    for (const auto &hazard : waterHazards_) {
        if (hazard.contains(destination)) {
            hazardTriggered = true;
            return start;
        }
    }

    if (remainingDistance(destination) < 0.4) {
        destination = cupPosition_;
    }

    return destination;
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

bool Hole::bounceAgainstWall(const Vector2D &start, Vector2D &destination, Vector2D &displacement) const {
    const double totalDisplacement = displacement.magnitude();
    if (totalDisplacement <= 0.0 || walls_.empty()) {
        return false;
    }

    const Wall *hitWall = nullptr;
    Vector2D impactPoint{0.0, 0.0};
    double bestDistance = std::numeric_limits<double>::infinity();

    for (const auto &wall : walls_) {
        Vector2D intersection{0.0, 0.0};
        if (wall.intersectsSegment(start, destination, intersection)) {
            const double distanceToImpact = (intersection - start).magnitude();
            if (distanceToImpact < bestDistance) {
                bestDistance = distanceToImpact;
                impactPoint = intersection;
                hitWall = &wall;
            }
        }
    }

    if (hitWall == nullptr) {
        return false;
    }

    const double remainingDistance = std::max(0.0, totalDisplacement - bestDistance);
    Vector2D incomingSegment = impactPoint - start;
    if (incomingSegment.magnitude() == 0.0) {
        incomingSegment = displacement;
    }
    Vector2D reflectedDirection = hitWall->reflect(incomingSegment);
    Vector2D finalDisplacement = reflectedDirection.scaled(remainingDistance);
    destination = impactPoint + finalDisplacement;
    displacement = destination - start;
    return true;
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