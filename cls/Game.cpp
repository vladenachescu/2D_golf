#include "Game.h"
#include <iostream>
#include <fstream>
using namespace std;

Game::Game(): scor(0) {}

void Game::start() {
    cout << "==============================\n";
    cout << "    JOC DE GOLF 2D - POO\n";
    cout << "==============================\n";

    ifstream fin("tastatura.txt");
    if (!fin.is_open()) {
        cerr << "Eroare: nu s-a putut deschide fisierul tastatura.txt\n";
        return;
    }

    int n;
    fin >> n;
    nivele.resize(n);

    for (int i = 0; i < n; i++) {
        nivele[i].incarca(fin, i + 1);
    }

    for (int i = 0; i < n; i++) {
        cout << "\n=== Nivel " << i + 1 << " ===\n";
        bool win = nivele[i].simuleaza();
        if (win) scor++;
    }

    cout << "\n=== Joc terminat! Scor final: " << scor << "/" << n << " ===\n";
}
