#include "Sand.h"
#include <iostream>
using namespace std;

Sand::Sand(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Sand::interact(Ball& b, const Vector2D&, float dt) {
    if (intersecteaza(b.getPoz())) {
        // Apply friction based on dt to prevent frame-rate dependency/instant stopping
        // Previous was 0.8 per frame. For 60fps, that's massive.
        // We want significant slowdown. 
        // V_new = V_old * (1 - friction * dt)
        float friction = 2.0f; // Tunable
        float factor = 1.0f - friction * dt;
        if (factor < 0) factor = 0;
        b.incetineste(factor);
        // std::cout << ">> Minge pe nisip! Viteza redusa.\n"; // Spam reduction
    }
}


void Sand::render(sf::RenderTarget& target) const {
    float w = (xmax - xmin) * 10.0f;
    float h = (ymax - ymin) * 10.0f;
    sf::RectangleShape rect(sf::Vector2f({w, h}));
    rect.setPosition({xmin * 10.0f, ymin * 10.0f});
    
    if (m_texture) {
        // Tiling logic: texture coordinates (0,0) to (w, h) will repeat the texture
        // assuming the texture is set to Repeated in Game.cpp
        rect.setTexture(m_texture);
        rect.setTextureRect(sf::IntRect({0, 0}, {(int)w, (int)h}));
        rect.setFillColor(sf::Color::White);
    } else {
        rect.setFillColor(sf::Color::Yellow);
    }

    
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(sf::Color::Black);
    
    target.draw(rect);
}

std::string Sand::tip() const { return "Nisip"; }
