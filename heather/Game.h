#pragma once
#include "Level.h"
#include <vector>
using namespace std;

class Game {
private:
    vector<Level> nivele;
    int scor;
public:
    Game();
    void start();
};
