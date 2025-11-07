#pragma once
#include "Obstacle.h"
using namespace std;
class Sand : public Obstacle {
public:
    Sand(float xmin, float xmax, float ymin, float ymax);
    void interact(Ball& b, const Vector2D& prev) override;
    string tip() const override;
};
