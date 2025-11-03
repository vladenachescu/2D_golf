#include "Water.h"
#include <iostream>
using namespace std;

Water::Water(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Water::interact(Ball& b, const Vector2D& prev) {
    if (intersecteazaLinie(prev, b.getPoz())) {
        cout << ">> Minge a trecut peste apa! Resetare pozitie...\n";
        b.reseteaza();
    }
}

string Water::tip() const { return "Apa"; }
