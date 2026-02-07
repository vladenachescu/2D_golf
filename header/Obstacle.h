#pragma once
#include "Vector2D.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
using namespace std;

class Obstacle {
protected:
    float xmin, xmax, ymin, ymax;
public:
    static int obstacleCount; // [NEW] Static member

    Obstacle(float xmin, float xmax, float ymin, float ymax);
    virtual ~Obstacle(); // [NEW] Implementation in cpp for counter

    // [NEW] Virtual Constructor (Prototype)
    [[nodiscard]] virtual std::shared_ptr<Obstacle> clone() const = 0;

    virtual void interact(Ball& b, const Vector2D& prev, float dt) = 0;
    virtual void render(sf::RenderTarget& target) const = 0;
    [[nodiscard]] virtual std::string tip() const = 0;
    [[nodiscard]] bool intersecteaza(const Vector2D& v) const;
    [[nodiscard]] bool intersecteazaLinie(const Vector2D& a, const Vector2D& b) const;

    // [NEW] NVI Pattern: Public interface controls printing
    void print(std::ostream& os) const {
        afisare(os);
    }

    // Texture support
    void setTexture(const sf::Texture* tex) { m_texture = tex; }
    
protected:
    const sf::Texture* m_texture = nullptr;

    // [NEW] Virtual implementation is protected (NVI)
    virtual std::ostream& afisare(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const Obstacle& o);
};
