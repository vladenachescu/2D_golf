#pragma once
#include "Level.h"
#include <iostream>

class Game {
private:
    // MODIFICARE: Folosim pointer pentru tablou dinamic (Regula celor Trei)
    Level* nivele;
    int nrNivele;
    int capacitate;

    int scor;

public:
    Game();

    ~Game();                            // 1. Destructor
    Game(const Game& other);            // 2. Constructor de Copiere
    Game& operator=(const Game& other); // 3. Operator de Atribuire
    void start();
    void adaugaNivel(const Level& nivel);
    void reseteazaJoc();
    void afiseazaScorFinal() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};