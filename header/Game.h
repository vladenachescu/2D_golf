#pragma once
#include "Level.h"
#include <iostream>
#include <vector> // <--- NECESAR PENTRU VECTOR

class Game {
private:
    // Inlocuim pointerul Level* cu un vector
    std::vector<Level> nivele;
    int scor;

public:

    Game();
    void start();
    void adaugaNivel(const Level& nivel);
    void afiseazaScorFinal() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};