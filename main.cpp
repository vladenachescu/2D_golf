#include "Game.h"
#include "Hole.h"
#include "Player.h"
#include "SandTrap.h"
#include "Shot.h"
#include "Vector2D.h"
#include "Wall.h"
#include "WaterHazard.h"

#include <exception>
#include <iostream>
#include <array>
#include <vector>

int main() {
    using namespace golf;

    try {
        Vector2D tee{0.0, 0.0};
        Vector2D cup{12.0, 3.5};
        std::cout << "Tee vector: " << tee << " magnitude=" << tee.magnitude() << '\n';
        std::cout << "Cup vector: " << cup << " normalized=" << cup.normalized() << '\n';
        std::cout << "Offset: " << (cup - tee) << " scaled=" << (cup - tee).scaled(0.5) << '\n';

        Shot previewShot{cup - tee, 4.5};
        std::cout << "Preview shot direction: " << previewShot.direction()
                  << ", power=" << previewShot.power()
                  << ", velocity=" << previewShot.velocity() << '\n';

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
        Hole first{"Sunny Start", tee, cup, sandTraps, waterHazards, walls};
        std::cout << first;
        std::cout << "Remaining from tee: " << first.remainingDistance(first.teePosition()) << '\n';
        std::cout << "Is tee in hazard? " << std::boolalpha << first.inHazard(first.teePosition()) << '\n';
        bool previewHazard = false;
        Vector2D previewLanding = first.simulateBallRoll(first.teePosition(), previewShot, 0.15, previewHazard);
        std::cout << "Preview landing position: " << previewLanding << ", hazard=" << previewHazard << "\n\n";

        std::vector<SandTrap> secondSand = {
            SandTrap{Vector2D{11.4, -2.2}, Vector2D{13.0, -0.8}, 0.4},
        };
        std::vector<WaterHazard> secondWater = {
            WaterHazard{Vector2D{14.4, -1.8}, Vector2D{15.6, -0.6}},
        };
        std::vector<Wall> secondWalls = {
            Wall{Vector2D{9.5, -2.5}, Vector2D{9.5, 1.0}},
            Wall{Vector2D{14.0, -0.5}, Vector2D{16.5, -0.5}},
        };
        Hole second{"Windy Turn", tee, Vector2D{18.0, -2.0}, secondSand, secondWater, secondWalls};
        std::cout << second;

        Player alice{"Alice", tee};
        std::cout << "Starting player: " << alice.name() << " at " << alice.position() << '\n';
        alice.startHole(first);
        Shot alicePlan = alice.planShotToward(first.cupPosition(), 0.7);
        bool aliceHazard = false;
        Vector2D aliceLanding = first.simulateBallRoll(alice.position(), alicePlan, 0.15, aliceHazard);
        alice.recordShot(alicePlan, aliceLanding, aliceHazard);
        std::cout << "Alice after preview shot: " << alice.position() << ", total score=" << alice.totalScore()
                  << ", hazard?=" << alice.lastShotWasHazard() << '\n';

        Player ben{"Ben", tee};

        GolfGame game({first, second}, 0.18);
        std::cout << "Initial friction: " << game.friction() << '\n';
        game.addPlayer(alice);
        game.addPlayer(ben);
        std::cout << "Game setup:\n" << game << '\n';

        game.playRound();
        std::cout << "After automated round:\n" << game << '\n';
    } catch (const std::exception &ex) {
        std::cerr << "Unexpected error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
