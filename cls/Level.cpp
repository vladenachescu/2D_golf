#include "Level.h"
#include <iostream>

void Level::incarca(int nrNivel) {
    std::cout << "\n=== Configurare nivel " << nrNivel << " ===\n";
    float bx, by;
    std::cout << "Pozitie start minge (x y): ";
    std::cin >> bx >> by;
    minge = Ball(Vector2D(bx, by));
    float hx, hy, hr;
    std::cout << "Pozitie si raza gaura (x y r): ";
    std::cin >> hx >> hy >> hr;
    gaura = Hole(Vector2D(hx, hy), hr);
    int nrObs;
    std::cout << "Numar obstacole: ";
    std::cin >> nrObs;
    obstacole.clear();
    for (int i = 0; i < nrObs; i++) {
        string tip; float xmin, xmax, ymin, ymax;
        std::cout << "  Obstacol " << i+1 << " (tip WALL/WATER/SAND si xmin xmax ymin ymax): ";
        std::cin >> tip >> xmin >> xmax >> ymin >> ymax;
        if (tip == "WALL") obstacole.push_back(make_shared<Wall>(xmin, xmax, ymin, ymax));
        else if (tip == "WATER") obstacole.push_back(make_shared<Water>(xmin, xmax, ymin, ymax));
        else obstacole.push_back(make_shared<Sand>(xmin, xmax, ymin, ymax));
    }
}

bool Level::simuleaza() {
    std::cout << "\n--- START NIVEL ---\n";
    std::cout << gaura << "\n";
    std::cout << "Obstacole:\n";
    for (const
        auto& o : obstacole) std::cout << "  - " << *o << "\n";
    int lovituri = 0;
    while (true) {
        float unghi, forta;
        std::cout << "\n[Lovitura " << ++lovituri << "] Unghi si forta (grade, valoare) sau -1 pentru iesire: ";
        if (unghi == -1 && forta == -1) return false;
        minge.loveste(forta, unghi);
        for (int t=0; t<300; t++) {
            Vector2D prev = minge.getPoz();
            minge.actualizeazaPozitia(0.1f);
            for (const auto& o : obstacole)
                o->interact(minge, prev);
            if (gaura.contine(minge.getPoz())) {
                std::cout << ">> Minge in gaura dupa " << lovituri << " lovituri!\n";
                return true;
            }
            if (minge.vitezaMica()) break;
        }
        std::cout << minge << "\n";
    }
}
