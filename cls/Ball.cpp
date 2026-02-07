#include "Ball.h"
#include <cmath>

namespace {
    const float kPi = 3.14159265358979323846f;
    constexpr float kEps = 1e-3f;
}

Ball::Ball()
    : pozitie(0,0), viteza(0,0), ultimaPozitie(0,0), raza(1.0f) {}

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
    // Damping factor adjusted for 20 substeps per frame.
    viteza = viteza * 0.999f; 
    
    // Hard stop to prevent "creeping" pixels at the end
    if (viteza.lungime() < 0.1f) {
        viteza = Vector2D(0,0);
    }
}

void Ball::incetineste(float factor) { viteza = viteza * factor; }
void Ball::reflectaX() { viteza = Vector2D(-viteza.getX(), viteza.getY()); }
void Ball::reflectaY() { viteza = Vector2D(viteza.getX(), -viteza.getY()); }
void Ball::reseteaza() { pozitie = ultimaPozitie; viteza = Vector2D(0,0); }
void Ball::aplicaAcceleratie(const Vector2D& acceleratie, float dt) {
    viteza = viteza + acceleratie * dt;
}

// Old method - resets velocity (used for teleporting/respawn)
void Ball::seteazaPozitia(const Vector2D& p) {
    pozitie = p;
    ultimaPozitie = p;
    viteza = Vector2D(0,0);
}

// New method - keeps velocity (used for collision resolution)
void Ball::setPoz(const Vector2D& p) {
    pozitie = p;
}

// Vector reflection: v = v - 2 * (v . n) * n
void Ball::reflecta(const Vector2D& normal) {
    float d = viteza.dot(normal);
    viteza = viteza - normal * (2.0f * d);
}
void Ball::adaugaViteza(const Vector2D& delta) {
    viteza = viteza + delta;
}
const Vector2D& Ball::getPoz() const { return pozitie; }
float Ball::getRaza() const { return raza; }
bool Ball::vitezaMica() const { return std::fabs(viteza.getX()) < kEps && std::fabs(viteza.getY()) < kEps; }

void Ball::render(sf::RenderTarget& target) const {
    if (m_texture && m_texture->getSize().x > 0) { // [FIX] Check size > 0
        sf::Sprite sprite(*m_texture);
        // Center origin
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        
        // Scale to match radius (if bounds.width > 0)
        float diameterPixels = raza * 2.0f * 10.0f;
        if (bounds.size.x > 0.001f) {
            float scale = diameterPixels / bounds.size.x;
            sprite.setScale({scale, scale});
        }
        
        sprite.setPosition({pozitie.getX() * 10.0f, pozitie.getY() * 10.0f});
        target.draw(sprite);
    } else {
        sf::CircleShape circle(raza * 10.0f); // Scaling for visualization
        circle.setFillColor(sf::Color::White);
        circle.setOrigin({raza * 10.0f, raza * 10.0f});
        circle.setPosition({pozitie.getX() * 10.0f, pozitie.getY() * 10.0f}); // Scaling position
        target.draw(circle);
    }
}

std::ostream& operator<<(std::ostream& os, const Ball& b) {
    os << "Pozitie minge: " << b.pozitie;
    return os;
}
