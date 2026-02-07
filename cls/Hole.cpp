#include "Hole.h"

Hole::Hole(Vector2D p, float r): poz(p), raza(r) {}
bool Hole::contine(const Vector2D& v) const { return poz.dist(v) <= raza; }
const Vector2D& Hole::getPoz() const { return poz; }

void Hole::render(sf::RenderTarget& target) const {
    sf::CircleShape circle(raza * 10.0f);
    circle.setFillColor(sf::Color::Black);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(1.5f); // Thin white outline as requested
    circle.setOrigin({raza * 10.0f, raza * 10.0f});
    circle.setPosition({poz.getX() * 10.0f, poz.getY() * 10.0f});
    
    // Ignore texture to force "simple black hole" look
    
    target.draw(circle);
    
    // Flag Pole
    sf::RectangleShape pole(sf::Vector2f({2.0f, 40.0f})); // 2px width, 40px height
    pole.setFillColor(sf::Color(100, 100, 100)); // Grey
    pole.setOrigin({1.0f, 40.0f}); // Origin at bottom center
    pole.setPosition({poz.getX() * 10.0f, poz.getY() * 10.0f - flagOffset}); // Apply offset
    pole.setOutlineThickness(1.0f);
    pole.setOutlineColor(sf::Color::Black);
    target.draw(pole);
    
    // Flag (Triangle)
    sf::ConvexShape flag(3);
    flag.setPoint(0, {0.0f, 0.0f});
    flag.setPoint(1, {15.0f, 5.0f});
    flag.setPoint(2, {0.0f, 10.0f});
    flag.setFillColor(sf::Color::Red);
    flag.setOutlineThickness(1.0f);
    flag.setOutlineColor(sf::Color::Black);
    // Position at top of pole
    flag.setPosition({poz.getX() * 10.0f, poz.getY() * 10.0f - 40.0f - flagOffset}); 
    target.draw(flag);
}

std::ostream& operator<<(std::ostream& os, const Hole& h) {
    os << "Gaura la " << h.poz << " (r=" << h.raza << ")";
    return os;
}
