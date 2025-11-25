#pragma once
#include "Vector2D.h"
#include "Ball.h"
#include <string>
#include <iostream>
using namespace std;

class Obstacle {
protected:
    float xmin, xmax, ymin, ymax;
public:
    Obstacle(float xmin, float xmax, float ymin, float ymax);
    virtual void interact(Ball& b, const Vector2D& prev, float dt) = 0;
    [[nodiscard]] virtual std::string tip() const = 0;
    [[nodiscard]] bool intersecteaza(const Vector2D& v) const;
    [[nodiscard]] bool intersecteazaLinie(const Vector2D& a, const Vector2D& b) const;
    virtual std::ostream& afisare(std::ostream& os) const;

    virtual ~Obstacle() = default;
    friend std::ostream& operator<<(std::ostream& os, const Obstacle& o);
};
