#include "Wall.h"
#include <iostream>


Wall::Wall(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Wall::interact(Ball& b, const Vector2D&, float) {
    if (intersecteaza(b.getPoz())) {
        b.reflectaX();
        b.reflectaY();
        std::cout << ">> Loveste peretele!\n";
    }
}

std::string Wall::tip() const { return "Perete"; }
