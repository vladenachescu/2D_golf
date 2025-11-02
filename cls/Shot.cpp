#include "Shot.h"

#include <ostream>

namespace golf {

    Shot::Shot(const Vector2D &direction, double power)
    : _direction{direction.normalized()}, _power{power} {}

    const Vector2D &Shot::direction() const { return _direction; }

    double Shot::power() const { return _power; }

    Vector2D Shot::velocity() const { return _direction.scaled(_power); }

    std::ostream &operator<<(std::ostream &os, const Shot &shot) {
        os << "Shot{dir=" << shot.direction() << ", power=" << shot.power() << "}";
        return os;
    }

}