#pragma once
#include "Vector2D.h"
#include "Ball.h"
#include <string>
using namespace std;

class Obstacle {
protected:
    float xmin, xmax, ymin, ymax;
public:
    Obstacle(float xmin, float xmax, float ymin, float ymax);
    virtual void interact(Ball& b, const Vector2D& prev) = 0;
    virtual string tip() const = 0;
    bool intersecteaza(const Vector2D& v) const;
    bool intersecteazaLinie(const Vector2D& a, const Vector2D& b) const;
    virtual ~Obstacle() = default;
    friend ostream& operator<<(ostream& os, const Obstacle& o);
};
