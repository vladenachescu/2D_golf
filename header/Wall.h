#pragma once
#include "Obstacle.h"

class Wall : public Obstacle {
public:
    Wall(float xmin, float xmax, float ymin, float ymax);
    void interact(Ball& b, const Vector2D& prev, float dt) override;
    [[nodiscard]] std::string tip() const override;
};
