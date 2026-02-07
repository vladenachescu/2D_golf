#include "Wall.h"
#include <iostream>
#include <cmath>
#include <algorithm>


Wall::Wall(float xmin, float xmax, float ymin, float ymax)
    : Obstacle(xmin, xmax, ymin, ymax) {}

void Wall::interact(Ball& b, const Vector2D&, float) {
    // AABB-Circle Collision Resolution
    Vector2D ballPos = b.getPoz();
    float r = b.getRaza();

    // 1. Find closest point on AABB to Circle Center
    float cx = std::max(xmin, std::min(ballPos.getX(), xmax));
    float cy = std::max(ymin, std::min(ballPos.getY(), ymax));
    
    Vector2D closest(cx, cy);
    Vector2D diff = ballPos - closest;
    float distSq = diff.getX()*diff.getX() + diff.getY()*diff.getY();
    
    // Check collision (distance < radius)
    // Note: if ball center is INSIDE AABB, distSq is 0. We must handle that.
    bool inside = (distSq < 0.0001f);
    
    if (inside || distSq < r*r) {
        // Collision detected
        
        // Penetration depth
        float dist = std::sqrt(distSq);
        Vector2D normal;
        float penetration = 0.0f;

        if (inside) {
            // Center is inside: find closest edge to push out
            float dLeft = ballPos.getX() - xmin;
            float dRight = xmax - ballPos.getX();
            float dTop = ballPos.getY() - ymin;
            float dBottom = ymax - ballPos.getY();
            
            // Find min
            float minD = std::min({dLeft, dRight, dTop, dBottom});
            
            if (minD == dLeft) normal = Vector2D(1, 0);       // Push Right
            else if (minD == dRight) normal = Vector2D(-1, 0); // Push Left
            else if (minD == dTop) normal = Vector2D(0, 1);    // Push Down
            else normal = Vector2D(0, -1);                     // Push Up
            
            penetration = r + minD; // Conservative push
        } else {
            // Center is outside: standard collision
            normal = diff.normalizat();
            penetration = r - dist;
        }
        
        // Anti-Tunneling / Stuck Fix:
        // If penetration is very deep (> radius), we might have tunneled. 
        // Push towards the Previous Position instead of the "closest edge" logic which might be wrong.
        // float distFromPrev = ballPos.dist(b.getUltimaPoz());
        // For now, let's just stick to the normal push but ensure we kill velocity component into the wall 
        // to prevent "fighting".
        
        // Push Ball Out (WITHOUT stopping it)
        b.setPoz(ballPos + normal * (penetration + 0.001f)); 
        
        // Use true vector reflection for accurate bounce off walls and corners
        b.reflecta(normal);
        
        // Ensure velocity is pointing AWAY from wall
        if (b.getPoz().dist(ballPos) > 0.001f) { 
            // If we moved the ball, double check velocity
             // Check dot product of new velocity and normal
        }
    }
}

std::string Wall::tip() const { return "Perete"; }

void Wall::render(sf::RenderTarget& target) const {
    float w = (xmax - xmin) * 10.0f;
    float h = (ymax - ymin) * 10.0f;
    sf::RectangleShape rect(sf::Vector2f({w, h}));
    rect.setPosition({xmin * 10.0f, ymin * 10.0f});
    
    if (m_texture) {
        rect.setTexture(m_texture);
        rect.setTextureRect(sf::IntRect({0, 0}, {(int)(w * 0.5f), (int)(h * 0.5f)}));
        rect.setFillColor(sf::Color::White); // Reset color to allow texture to show full brightness
    } else {
        rect.setFillColor(sf::Color(100, 100, 100)); // Grey for walls
    }

    
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(sf::Color::Black);
    
    target.draw(rect);
}
