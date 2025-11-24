#include "Game.h"
#include <iostream>
using namespace std;


Game::Game() : nivele(nullptr), nrNivele(0), capacitate(0), scor(0) {}

Game::Game(int capacitateInitiala)
    : nivele(nullptr), nrNivele(0), capacitate(capacitateInitiala), scor(0) {
    if (capacitate > 0) {
        nivele = new Level[capacitate];
    } else {
        capacitate = 0;
    }
    std::cout << ">> (Debug) Game creat cu capacitate initiala: " << capacitate << "\n";
}

Game::~Game() {
    delete[] nivele;
}

Game::Game(const Game& other)
    : nrNivele(other.nrNivele), capacitate(other.capacitate), scor(other.scor) {

    // Alocam memorie separata
    nivele = new Level[capacitate];
    // Copiem fiecare nivel
    for (int i = 0; i < nrNivele; ++i) {
        nivele[i] = other.nivele[i];
    }
    std::cout << ">> (Debug) Copy Constructor apelat.\n";
}

Game& Game::operator=(const Game& other) {
    if (this == &other) {
        return *this;
    }

    delete[] nivele;

    nrNivele = other.nrNivele;
    capacitate = other.capacitate;
    scor = other.scor;

    nivele = new Level[capacitate];
    for (int i = 0; i < nrNivele; ++i) {
        nivele[i] = other.nivele[i];
    }

    return *this;
}

void Game::adaugaNivel(const Level& nivel) {
    if (nrNivele == capacitate) {
        int newCap = (capacitate == 0) ? 2 : capacitate * 2;
        Level* temp = new Level[newCap];

        for (int i = 0; i < nrNivele; ++i) {
            temp[i] = nivele[i];
        }

        delete[] nivele;
        nivele = temp;
        capacitate = newCap;
    }

    nivele[nrNivele] = nivel;
    nrNivele++;
}



void Game::afiseazaScorFinal() const {
    std::cout << "Scor final: " << scor << "/" << nrNivele << "\n";
}

void Game::start() {
    std::cout << "==============================\n";
    std::cout << "    JOC DE GOLF 2D - POO\n";
    std::cout << "==============================\n";

    int n;
    std::cout << "Cate nivele vrei sa joci? ";
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        Level nivelTemp;
        nivelTemp.incarca(i + 1, cin);
        this->adaugaNivel(nivelTemp);
    }

    for (int i = 0; i < nrNivele; i++) {
        std::cout << "\n=== Nivel " << i+1 << " ===\n";
        bool win = nivele[i].simuleaza(std::cin);
        if (win) scor++;
    }

    std::cout << "\n=== Joc terminat! ===\n";
    afiseazaScorFinal();
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game (Nivele: " << game.nrNivele << ", Capacitate: " << game.capacitate
       << ", Scor: " << game.scor << ")";
    return os;
}