#pragma once

#include "SandTrap.h"
#include "Shot.h"
#include "Vector2D.h"
#include "Wall.h"
#include "WaterHazard.h"

#include <string>
#include <vector>

namespace golf {

    class Hole {
    public:
        Hole(std::string name, const Vector2D &teePosition, const Vector2D &cupPosition,
             std::vector<SandTrap> sandTraps, std::vector<WaterHazard> waterHazards, std::vector<Wall> walls);

        const std::string &name() const;
        const Vector2D &teePosition() const;
        const Vector2D &cupPosition() const;
        double remainingDistance(const Vector2D &ballPosition) const;
        bool inHazard(const Vector2D &position) const;
        Vector2D simulateBallRoll(const Vector2D &start, const Shot &shot, double friction,
                                  bool &hazardTriggered) const;

    private:
        friend std::ostream &operator<<(std::ostream &os, const Hole &hole);

        std::string name_;
        Vector2D teePosition_;
        Vector2D cupPosition_;
        std::vector<SandTrap> sandTraps_;
        std::vector<WaterHazard> waterHazards_;
        std::vector<Wall> walls_;

        bool isInSand(const Vector2D &position) const;
        bool isInWater(const Vector2D &position) const;
        bool bounceAgainstWall(const Vector2D &start, Vector2D &destination, Vector2D &displacement) const;
    };

    std::ostream &operator<<(std::ostream &os, const Hole &hole);

}