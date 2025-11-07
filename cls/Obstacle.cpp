#include "Obstacle.h"
#include <algorithm>

Obstacle::Obstacle(float xmin, float xmax, float ymin, float ymax)
    : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax) {}

bool Obstacle::intersecteaza(const Vector2D& v) const {
    return (v.getX() >= xmin && v.getX() <= xmax &&
            v.getY() >= ymin && v.getY() <= ymax);
}

bool Obstacle::intersecteazaLinie(const Vector2D& a, const Vector2D& b) const {
    float aminX = std::min(a.getX(), b.getX());
    float amaxX = std::max(a.getX(), b.getX());
    float aminY = std::min(a.getY(), b.getY());
    float amaxY = std::max(a.getY(), b.getY());
    return !(amaxX < xmin || aminX > xmax || amaxY < ymin || aminY > ymax);
}

std::ostream& operator<<(std::ostream& os, const Obstacle& o) {
    os << o.tip() << " [" << o.xmin << "," << o.ymin << "] -> ["
       << o.xmax << "," << o.ymax << "]";
    return os;
}
