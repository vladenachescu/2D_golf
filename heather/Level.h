#pragma once
#include "Ball.h"
#include "Hole.h"
#include "Water.h"
#include <vector>
#include <memory>
using namespace std;

class Level {
private:
    Ball minge;
    Hole gaura;
    vector<shared_ptr<Obstacle>> obstacole;
    vector<pair<float,float>> lovituriData;
public:
    void incarca(ifstream& fin, int nrNivel);
    bool simuleaza();
};
