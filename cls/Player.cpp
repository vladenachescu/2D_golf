#include "Player.h"

#include <cmath>
#include <ostream>
#include <numeric>
#include <utility>

namespace golf {

Player::Player(std::string name, const Vector2D &initialPosition)
    : name_{std::move(name)}, position_{initialPosition}, strokes_{}, shotHistory_{}, hazardHistory_{} {}

Player::Player(const Player &other)
    : name_{other.name_},
      position_{other.position_},
      strokes_{other.strokes_},
      shotHistory_{other.shotHistory_},
      hazardHistory_{other.hazardHistory_} {}

Player &Player::operator=(const Player &other) {
    if (this != &other) {
        name_ = other.name_;
        position_ = other.position_;
        strokes_ = other.strokes_;
        shotHistory_ = other.shotHistory_;
        hazardHistory_ = other.hazardHistory_;
    }
    return *this;
}

Player::~Player() noexcept = default;

const std::string &Player::name() const { return name_; }

const Vector2D &Player::position() const { return position_; }

void Player::startHole(const Hole &hole) {
    position_ = hole.teePosition();
}

Shot Player::planShotToward(const Vector2D &target, double strengthFactor) const {
    const Vector2D toTarget = target - position_;
    const double distance = toTarget.magnitude();
    const double power = distance * strengthFactor;
    return Shot{toTarget, power};
}

void Player::recordShot(const Shot &shot, const Vector2D &newPosition, bool hazardTriggered) {
    shotHistory_.push_back(shot);
    position_ = newPosition;
    strokes_.push_back(1.0);
    hazardHistory_.push_back(hazardTriggered);
}

double Player::totalScore() const {
    return std::accumulate(strokes_.begin(), strokes_.end(), 0.0);
}

bool Player::lastShotWasHazard() const {
    if (hazardHistory_.empty()) {
        return false;
    }
    return hazardHistory_.back();
}

std::ostream &operator<<(std::ostream &os, const Player &player) {
    os << "Player '" << player.name() << "' at " << player.position() << " with "
       << player.shotHistory_.size() << " shots";
    if (!player.shotHistory_.empty()) {
        os << "\n  Last shot: " << player.shotHistory_.back();
        os << " (hazard=" << std::boolalpha << player.lastShotWasHazard() << ")";
    }
    os << "\n  Total score: " << player.totalScore() << '\n';
    return os;
}

}