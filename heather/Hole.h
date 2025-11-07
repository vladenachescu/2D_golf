#pragma once
#include "Vector2D.h"
using namespace std;
class Hole {
private:
    Vector2D poz;
    float raza;
public:
    explicit Hole(Vector2D p = Vector2D(10,0), float r=0.5f);
    bool contine(const Vector2D& v) const;
    const Vector2D& getPoz() const;
    friend ostream& operator<<(ostream& os, const Hole& h);
};
