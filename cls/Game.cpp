#include "Game.h"
#include <iostream>
using namespace std;

Game::Game() : scor(0) {}

void Game::start() {
    std::cout << "==============================\n";
    std::cout << "    JOC DE GOLF 2D - POO\n";
    std::cout << "==============================\n";

    int n;
    std::cout << "Cate nivele vrei sa joci? ";
    std::cin >> n;
    nivele.resize(n);

    for (int i = 0; i < n; i++)
        nivele[i].incarca(i + 1, cin);

    for (int i = 0; i < n; i++) {
        std::cout << "\n=== Nivel " << i+1 << " ===\n";
        bool win = nivele[i].simuleaza(std::cin);
        if (win) scor++;
    }

    std::cout << "\n=== Joc terminat! Scor final: " << scor << "/" << n << " ===\n";
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game cu " << game.nivele.size() << " niveluri, scor curent " << game.scor;
    return os;
}
