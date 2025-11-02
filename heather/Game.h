#pragma once

#include "Hole.h"
#include "Player.h"

#include <vector>

namespace golf {

    class GolfGame {
    public:
        GolfGame(std::vector<Hole> holes, double friction);

        void addPlayer(const Player &player);
        void playRound();
        double friction() const;

    private:
        friend std::ostream &operator<<(std::ostream &os, const GolfGame &game);

        void resolveHole(Player &player, Hole &hole);

        std::vector<Hole> holes_;
        std::vector<Player> players_;
        double friction_;
    };

    std::ostream &operator<<(std::ostream &os, const GolfGame &game);

}