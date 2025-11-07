#include "Game.h"
#include <iostream>


Game::Game(): scor(0) {}

void Game::start() {

    std::cout << "==============================\n";
    std::cout << "    JOC DE GOLF 2D - POO\n";
    std::cout << "==============================\n";
    int n;
    std::cout << "Cate nivele vrei sa joci? ";
    std::cin >> n;
    nivele.resize(n);
    for (int i = 0; i < n; i++) nivele[i].incarca(i+1);
    for (int i = 0; i < n; i++) {
        cout << "\n=== Nivel " << i+1 << " ===\n";
        bool win = nivele[i].simuleaza();
        if (win) scor++;
    }
    cout << "\n=== Joc terminat! Scor final: " << scor << "/" << n << " ===\n";
}
