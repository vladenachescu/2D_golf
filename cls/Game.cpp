#include "Game.h"
#include <iostream>
using namespace std;

Game::Game() : scor(0) {}

void Game::start() {
    cout << "==============================\n";
    cout << "    JOC DE GOLF 2D - POO\n";
    cout << "==============================\n";

    int n;
    cout << "Cate nivele vrei sa joci? ";
    cin >> n;
    nivele.resize(n);

    for (int i = 0; i < n; i++)
        nivele[i].incarca(i + 1, cin);

    for (int i = 0; i < n; i++) {
        cout << "\n=== Nivel " << i + 1 << " ===\n";
        bool win = nivele[i].simuleaza(cin);
        if (win) scor++;
    }

    cout << "\n=== Joc terminat! Scor final: " << scor << "/" << n << " ===\n";
}
