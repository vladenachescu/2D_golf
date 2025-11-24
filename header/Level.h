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
    Hole gaura;
    std::vector<std::shared_ptr<Obstacle>> obstacole;

public:
    Level() = default;
    Level(const Ball& minge, const Hole& gaura, std::vector<std::shared_ptr<Obstacle>> obstacole);

    void incarca(int nrNivel, std::istream& in);
    bool simuleaza(std::istream& in);

    // MODIFICARE: Functii adaugate pentru a permite testarea in main()
    void adaugaObstacol(const std::shared_ptr<Obstacle>& obs);
    void reseteazaMinge();
    Ball& getMinge(); // Returnam referinta ca sa o putem modifica in teste

};