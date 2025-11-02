#include "Game.h"

#include <iomanip>
#include <ostream>
#include <utility>

namespace golf {

    GolfGame::GolfGame(std::vector<Hole> holes, double friction)
        : holes_{std::move(holes)}, players_{}, friction_{friction} {}

    void GolfGame::addPlayer(const Player &player) {
        players_.push_back(player);
    }

    double GolfGame::friction() const { return friction_; }

    void GolfGame::playRound() {
        for (auto &player : players_) {
            for (auto &hole : holes_) {
                resolveHole(player, hole);
            }
        }
    }

    void GolfGame::resolveHole(Player &player, Hole &hole) {
        player.startHole(hole);
        size_t safetyCounter = 0;
        while (hole.remainingDistance(player.position()) > 0.1 && safetyCounter < 12) {
            const double progressFactor = hole.remainingDistance(player.position()) < 3.0 ? 0.9 : 0.6;
            const Shot shot = player.planShotToward(hole.cupPosition(), progressFactor);
            bool hazard = false;
            const Vector2D nextPosition = hole.simulateBallRoll(player.position(), shot, friction_, hazard);
            player.recordShot(shot, nextPosition, hazard);
            ++safetyCounter;
        }
    }

    std::ostream &operator<<(std::ostream &os, const GolfGame &game) {
        os << std::fixed << std::setprecision(2);
        os << "Golf game with " << game.friction() << " friction\n";
        for (const auto &hole : game.holes_) {
            os << hole;
        }
        os << "Participants:";
        for (const auto &player : game.players_) {
            os << '\n' << player;
        }
        return os;
    }

}