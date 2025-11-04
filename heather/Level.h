#pragma once
#include "Ball.h"
#include "Hole.h"
#include "Wall.h"
#include "Water.h"
#include "Sand.h"
#include <vector>
#include <memory>
using namespace std;

class Level {
private:
    Ball minge;
    Hole gaura;
    vector<shared_ptr<Obstacle>> obstacole;
public:
    void incarca(int nrNivel);
    bool simuleaza();
};
