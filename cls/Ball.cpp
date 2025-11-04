#include "Ball.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr float EPS = 1e-3f;

Ball::Ball()
    : pozitie(0,0), viteza(0,0), ultimaPozitie(0,0), raza(0.2f) {}

Ball::Ball(const Vector2D& p, float r)
    : pozitie(p), viteza(0,0), ultimaPozitie(p), raza(r) {}

void Ball::loveste(float forta, float unghi) {
    float rad = unghi * M_PI / 180.0f;
    viteza = Vector2D(cos(rad) * forta, sin(rad) * forta);
    ultimaPozitie = pozitie;
}

void Ball::actualizeazaPozitia(float dt) {
    pozitie = pozitie + viteza * dt;
    viteza = viteza * 0.98f;
}

void Ball::incetineste(float factor) { viteza = viteza * factor; }
void Ball::reflectaX() { viteza = Vector2D(-viteza.getX(), viteza.getY()); }
void Ball::reflectaY() { viteza = Vector2D(viteza.getX(), -viteza.getY()); }
void Ball::reseteaza() { pozitie = ultimaPozitie; viteza = Vector2D(0,0); }
const Vector2D& Ball::getPoz() const { return pozitie; }
const Vector2D& Ball::getUltimaPoz() const { return ultimaPozitie; }
bool Ball::vitezaMica() const { return fabs(viteza.getX()) < EPS && fabs(viteza.getY()) < EPS; }
ostream& operator<<(ostream& os, const Ball& b) {
    os << "Pozitie minge: " << b.pozitie;
    return os;
}
