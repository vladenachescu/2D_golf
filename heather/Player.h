#pragma once

#include "Hole.h"
#include "Shot.h"
#include "Vector2D.h"

#include <string>
#include <vector>

namespace golf {

    class Player {
    public:
        Player(std::string name, const Vector2D &initialPosition);
        Player(const Player &other);
        Player &operator=(const Player &other);
        ~Player() noexcept;

        const std::string &name() const;
        const Vector2D &position() const;
        void startHole(const Hole &hole);
        Shot planShotToward(const Vector2D &target, double strengthFactor) const;
        void recordShot(const Shot &shot, const Vector2D &newPosition, bool hazardTriggered);
        double totalScore() const;
        bool lastShotWasHazard() const;
        void resetPositionToLastSafe();

    private:
        friend std::ostream &operator<<(std::ostream &os, const Player &player);

        std::string name_;
        Vector2D position_;
        std::vector<double> strokes_;
        std::vector<Shot> shotHistory_;
        std::vector<bool> hazardHistory_;
    };

    std::ostream &operator<<(std::ostream &os, const Player &player);

}