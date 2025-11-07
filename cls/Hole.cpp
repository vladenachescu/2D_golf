#include "Hole.h"

Hole::Hole(Vector2D p, float r): poz(p), raza(r) {}
bool Hole::contine(const Vector2D& v) const { return poz.dist(v) <= raza; }
const Vector2D& Hole::getPoz() const { return poz; }
std::ostream& operator<<(std::ostream& os, const Hole& h) {
    os << "Gaura la " << h.poz << " (r=" << h.raza << ")";
    return os;
}
