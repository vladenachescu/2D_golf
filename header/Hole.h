#pragma once
#include "Vector2D.h"
#include <SFML/Graphics.hpp>

class Hole {
private:
    Vector2D poz;
    float raza;
public:
    explicit Hole(Vector2D p = Vector2D(10,0), float r=0.5f);
    bool contine(const Vector2D& v) const;
    const Vector2D& getPoz() const;
    void render(sf::RenderTarget& target) const;
    void setTexture(const sf::Texture* tex) { m_texture = tex; }
    void setFlagOffset(float offset) { flagOffset = offset; }
    
    friend std::ostream& operator<<(std::ostream& os, const Hole& h);
private:
    const sf::Texture* m_texture = nullptr;
    float flagOffset = 0.0f;
};
