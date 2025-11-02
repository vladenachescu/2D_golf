#include "Game.h"
#include "Hole.h"
#include "Player.h"
#include "SandTrap.h"
#include "Shot.h"
#include "Vector2D.h"
#include "Wall.h"
#include "WaterHazard.h"

#include <iostream>
#include <vector>

using namespace golf;

// --- Secțiune 1: funcții helper pentru testare ---

void previewHoleSimulation(const Hole &hole, const Shot &shot, double friction) {
    bool inHazard = false;
    std::cout << "\n=== Preview pentru " << hole.name() << " ===\n";
    std::cout << hole;

    std::cout << "Distanta ramasa de la tee: "
              << hole.remainingDistance(hole.teePosition()) << '\n';
    std::cout << "Tee in hazard? " << std::boolalpha
              << hole.inHazard(hole.teePosition()) << '\n';

    Vector2D landing = hole.simulateBallRoll(hole.teePosition(), shot, friction, inHazard);
    std::cout << "Pozitie finala estimata: " << landing
              << ", hazard=" << inHazard << "\n\n";
}

void simulatePlayerOnHole(Player &player, const Hole &hole, double friction) {
    std::cout << "\n=== Simulare pentru " << player.name() << " ===\n";
    player.startHole(hole);

    Shot shot = player.planShotToward(hole.cupPosition(), 1.0);
    bool hazard = false;
    Vector2D landing = hole.simulateBallRoll(player.position(), shot, friction, hazard);
    player.recordShot(shot, landing, hazard);

    std::cout << "Dupa lovitura: " << player << "\n";
}

// --- Secțiune 2: funcția main() simplificată ---

int main() {
    try {
        // Puncte de start și final
        Vector2D tee{0.0, 0.0};
        Vector2D cup{12.0, 3.5};

        // Obstacolele
        std::vector<SandTrap> sandTraps = {
            SandTrap{Vector2D{3.2, 0.8}, Vector2D{4.8, 2.2}, 0.45},
            SandTrap{Vector2D{9.9, 2.5}, Vector2D{10.9, 3.7}, 0.35},
        };

        std::vector<WaterHazard> waterHazards = {
            WaterHazard{Vector2D{8.0, 1.9}, Vector2D{9.2, 3.1}},
        };

        std::vector<Wall> walls = {
            Wall{Vector2D{5.5, -1.0}, Vector2D{5.5, 3.5}},
            Wall{Vector2D{7.5, 0.5}, Vector2D{9.0, 2.0}},
        };

        // Definim găurile
        Hole practice{"Sunny Start", tee, cup, sandTraps, waterHazards, walls};
        Hole aceHole{"Ace Lane", tee, Vector2D{6.0, 0.0}, {}, {}, {}};

        // Simulare rapidă a unei lovituri
        Shot previewShot{cup - tee, 4.5};
        previewHoleSimulation(practice, previewShot, 0.15);

        // Jucătorul
        Player vlad{"Vlad", tee};
        simulatePlayerOnHole(vlad, aceHole, 0.0);

        // Jocul complet
        GolfGame game({aceHole}, 0.0);
        game.addPlayer(vlad);

        std::cout << "\n=== Rezumatul jocului ===\n" << game << '\n';
    } catch (const std::exception &ex) {
        std::cerr << "Eroare: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
