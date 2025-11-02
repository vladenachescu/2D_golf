#pragma once
#include "Vector2D.h"
#include <iosfwd>
namespace golf {
    class Shot {
        public:
        Shot(const Vector2D &direction, double power);

        const Vector2D &direction() const;
        double power() const;
        Vector2D velocity() const;

        private:
         Vector2D _direction;
         double _power;
    };

    std::ostream &operator<<(std::ostream &os, const Shot &s);

}