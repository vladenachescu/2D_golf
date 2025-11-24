#pragma once
#include "Level.h"
#include <iostream>

class Game {
    Level* nivele;
    int nrNivele;
    int capacitate;
    int scor;

public:
    Game();
    explicit Game(int capacitateInitiala);
    ~Game();
    Game(const Game& other);
    Game& operator=(const Game& other);

    void start();
    void adaugaNivel(const Level& nivel);
    void reseteazaJoc();
    void afiseazaScorFinal() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};