#include "Water.h"
#include <iostream>


Water::Water(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Water::interact(Ball& b, const Vector2D& prev, float) {
    if (intersecteazaLinie(prev, b.getPoz())) {
        std::cout << ">> Minge a trecut peste apa! Resetare pozitie...\n";
        b.reseteaza();
    }
}


void Water::render(sf::RenderTarget& target) const {
    float w = (xmax - xmin) * 10.0f;
    float h = (ymax - ymin) * 10.0f;
    sf::RectangleShape rect(sf::Vector2f({w, h}));
    rect.setPosition({xmin * 10.0f, ymin * 10.0f});
    
    if (m_texture) {
        rect.setTexture(m_texture);
        rect.setTextureRect(sf::IntRect({0, 0}, {(int)w, (int)h}));
        rect.setFillColor(sf::Color::White);
    } else {
        rect.setFillColor(sf::Color::Blue);
    }
    
    // Visual Polish: Outline
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(sf::Color::Black);
    target.draw(rect);
}

std::string Water::tip() const { return "Apa"; }
