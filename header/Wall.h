#pragma once
#include "Obstacle.h"

class Wall : public Obstacle {
public:
    Wall(float xmin, float xmax, float ymin, float ymax);
    void interact(Ball& b, const Vector2D& prev, float dt) override;
    void render(sf::RenderTarget& target) const override;
    [[nodiscard]] std::string tip() const override;
    
    // [NEW] Virtual Constructor
    [[nodiscard]] std::shared_ptr<Obstacle> clone() const override {
        return std::make_shared<Wall>(*this);
    }
};
