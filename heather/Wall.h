#pragma once
#include "Obstacle.h"
using namespace std;
class Wall : public Obstacle {
public:
    Wall(float xmin, float xmax, float ymin, float ymax);
    void interact(Ball& b, const Vector2D& prev) override;
    string tip() const override;
};
