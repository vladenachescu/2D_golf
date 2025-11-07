#include "Sand.h"
#include <iostream>
using namespace std;

Sand::Sand(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Sand::interact(Ball& b, const Vector2D&, float) {
    if (intersecteaza(b.getPoz())) {
        b.incetineste(0.8f);
        std::cout << ">> Minge pe nisip! Viteza redusa.\n";
    }
}

std::string Sand::tip() const { return "Nisip"; }
