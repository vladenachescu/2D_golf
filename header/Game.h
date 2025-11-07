#pragma once
#include "Level.h"
#include <vector>
using namespace std;

class Game {
private:
    std::vector<Level> nivele;
    int scor;
public:
    Game();
    void start();
    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};
