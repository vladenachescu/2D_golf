#include "Game.h"
#include <iostream>


Game::Game() : scor(0) {
    std::cout << ">> (Debug) Game creat (folosind vector).\n";
}

void Game::adaugaNivel(const Level& nivel) {

    nivele.push_back(nivel);
}

void Game::afiseazaScorFinal() const {
    std::cout << "Scor final: " << scor << "/" << nivele.size() << "\n";
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
        nivelTemp.incarca(i + 1, std::cin);
        this->adaugaNivel(nivelTemp);
    }
    for (size_t i = 0; i < nivele.size(); ++i) {
        std::cout << "\n=== START JOC NIVEL " << i+1 << " ===\n";
        bool win = nivele[i].simuleaza(std::cin);

        if (win) scor++;
    }

    afiseazaScorFinal();
    std::cout << "\n=== Joc terminat! ===\n";
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game (Nivele: " << game.nivele.size() << ", Scor: " << game.scor << ")\n";


    for(size_t i = 0; i < game.nivele.size(); ++i) {
        os << "Detaliu Nivel " << i+1 << ":\n";
        os << game.nivele[i] << "\n";
    }

    return os;
}