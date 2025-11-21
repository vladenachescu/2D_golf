#include "Game.h"
#include <iostream>
using namespace std;

// Constructor default
Game::Game() : nivele(nullptr), nrNivele(0), capacitate(0), scor(0) {}

// --- IMPLEMENTARE REGULA CELOR TREI ---

// 1. Destructor
Game::~Game() {
    delete[] nivele; // Eliberam memoria
}

// 2. Constructor de Copiere
Game::Game(const Game& other)
    : nrNivele(other.nrNivele), capacitate(other.capacitate), scor(other.scor) {

    // Alocam memorie noua
    nivele = new Level[capacitate];
    // Copiem elementele unul cate unul
    for (int i = 0; i < nrNivele; ++i) {
        nivele[i] = other.nivele[i];
    }
    std::cout << ">> (Debug) Game: Copy Constructor apelat.\n";
}

// 3. Operator de Atribuire
Game& Game::operator=(const Game& other) {
    if (this == &other) {
        return *this; // Protectie la auto-atribuire (g = g)
    }

    // Pas 1: Eliberam memoria veche
    delete[] nivele;

    // Pas 2: Copiem datele primitive
    nrNivele = other.nrNivele;
    capacitate = other.capacitate;
    scor = other.scor;

    // Pas 3: Alocam memorie noua si copiem datele complexe
    nivele = new Level[capacitate];
    for (int i = 0; i < nrNivele; ++i) {
        nivele[i] = other.nivele[i];
    }

    std::cout << ">> (Debug) Game: Copy Assignment apelat.\n";
    return *this;
}
// --------------------------------------

void Game::adaugaNivel(const Level& nivel) {
    // Logica de redimensionare vector dinamic (netriviala)
    if (nrNivele == capacitate) {
        int newCap = (capacitate == 0) ? 2 : capacitate * 2;
        Level* newNivele = new Level[newCap];

        for (int i = 0; i < nrNivele; ++i) {
            newNivele[i] = nivele[i];
        }

        delete[] nivele;
        nivele = newNivele;
        capacitate = newCap;
    }

    nivele[nrNivele] = nivel;
    nrNivele++;
}

void Game::reseteazaJoc() {
    scor = 0;
    std::cout << "Joc resetat. Scorul este acum 0.\n";
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
        this->adaugaNivel(nivelTemp); // Folosim functia noastra, nu push_back
    }

    for (int i = 0; i < nrNivele; i++) {
        std::cout << "\n=== Nivel " << i+1 << " ===\n";
        // Aici se face copierea cand nivelul e returnat, deci Level trebuie sa fie copiabil
        bool win = nivele[i].simuleaza(std::cin);
        if (win) scor++;
    }

    std::cout << "\n=== Joc terminat! ===\n";
    afiseazaScorFinal();
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game cu " << game.nrNivele << " niveluri, scor curent " << game.scor;
    return os;
}