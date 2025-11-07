#pragma once
#include "Obstacle.h"

class Water : public Obstacle {
public:
    Water(float xmin, float xmax, float ymin, float ymax);
    void interact(Ball& b, const Vector2D& prev, float dt) override;
    std::string tip() const override;
};
