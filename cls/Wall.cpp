#include "Wall.h"
#include <iostream>
using namespace std;

Wall::Wall(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Wall::interact(Ball& b, const Vector2D&) {
    if (intersecteaza(b.getPoz())) {
        b.reflectaX();
        b.reflectaY();
        std::cout << ">> Loveste peretele!\n";
    }
}

string Wall::tip() const { return "Perete"; }
