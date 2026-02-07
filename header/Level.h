#pragma once
#include "Ball.h"
#include "Hole.h"
#include "Wall.h"
#include "Water.h"
#include "Sand.h"
#include <vector>
#include <memory>
#include <iostream>
#include "BlackHole.h"
#include "WhiteHole.h"
#include <istream>

class Level {
private:
    Ball minge;
    Vector2D startPoz;
    Hole gaura;
    std::vector<std::shared_ptr<Obstacle>> obstacole;

public:
    Level() = default;
    Level(const Ball& minge, const Hole& gaura, std::vector<std::shared_ptr<Obstacle>> obstacole);
    
    // [NEW] Rule of 5
    ~Level() = default; // Destructor
    Level(const Level& other); // Copy Const
    Level& operator=(const Level& other); // Copy Assign
    Level(Level&& other) noexcept = default; // Move Const
    Level& operator=(Level&& other) noexcept = default; // Move Assign


    void incarca(int nrNivel, std::istream& in);
    // Remove simuleaza, replace with update/render
    // bool simuleaza(std::istream& in); 
    void update(float dt);
    void render(sf::RenderTarget& target);
    
    // Helper to check win condition
    bool isFinished() const;

    void adaugaObstacol(const std::shared_ptr<Obstacle>& obs);
    void reseteazaMinge();
    Ball& getMinge();
    const Vector2D& getStartPoz() const { return startPoz; }
    
    // Texture assignment helpers
    const std::vector<std::shared_ptr<Obstacle>>& getObstacole() const { return obstacole; }
    Hole& getHole() { return gaura; }

    friend std::ostream& operator<<(std::ostream& os, const Level& level);

};