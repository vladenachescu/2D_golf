#include "Ball.h"
#include <cmath>

namespace {
    const float kPi = 3.14159265358979323846f;
    constexpr float kEps = 1e-3f;
}

Ball::Ball()
    : pozitie(0,0), viteza(0,0), ultimaPozitie(0,0), raza(0.2f) {}

Ball::Ball(const Vector2D& p, float r)
    : pozitie(p), viteza(0,0), ultimaPozitie(p), raza(r) {}

Ball::Ball(const Ball& other) = default;

Ball& Ball::operator=(const Ball& other) = default;

Ball::~Ball() = default;

void Ball::loveste(float forta, const float unghi) {
    float rad = kPi * unghi / 180.0f;
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
void Ball::aplicaAcceleratie(const Vector2D& acceleratie, float dt) {
    viteza = viteza + acceleratie * dt;
}
void Ball::seteazaPozitia(const Vector2D& p) {
    pozitie = p;
    ultimaPozitie = p;
    viteza = Vector2D(0,0);
}
void Ball::adaugaViteza(const Vector2D& delta) {
    viteza = viteza + delta;
}
const Vector2D& Ball::getPoz() const { return pozitie; }
bool Ball::vitezaMica() const { return std::fabs(viteza.getX()) < kEps && std::fabs(viteza.getY()) < kEps; }
std::ostream& operator<<(std::ostream& os, const Ball& b) {
    os << "Pozitie minge: " << b.pozitie;
    return os;
}
