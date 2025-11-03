#include "Level.h"
#include <iostream>
using namespace std;

void Level::incarca(ifstream& fin, int nrNivel) {
    cout << "\n=== Configurare nivel " << nrNivel << " ===\n";

    float bx, by;
    fin >> bx >> by;
    minge = Ball(Vector2D(bx, by));

    float hx, hy, hr;
    fin >> hx >> hy >> hr;
    gaura = Hole(Vector2D(hx, hy), hr);

    int nrObs;
    fin >> nrObs;
    obstacole.clear();

    for (int i = 0; i < nrObs; i++) {
        string tip; float xmin, xmax, ymin, ymax;
        fin >> tip >> xmin >> xmax >> ymin >> ymax;
        if (tip == "WALL") obstacole.push_back(make_shared<Wall>(xmin, xmax, ymin, ymax));
        else if (tip == "WATER") obstacole.push_back(make_shared<Water>(xmin, xmax, ymin, ymax));
        else obstacole.push_back(make_shared<Sand>(xmin, xmax, ymin, ymax));
    }

    // Citim toate loviturile pentru nivel
    int nrLovituri;
    fin >> nrLovituri;
    lovituriData.clear();
    for (int i = 0; i < nrLovituri; i++) {
        float unghi, forta;
        fin >> unghi >> forta;
        lovituriData.push_back({unghi, forta});
    }
}

bool Level::simuleaza() {
    cout << "\n--- START NIVEL ---\n";
    cout << gaura << "\n";
    cout << "Obstacole:\n";
    for (auto& o : obstacole) cout << "  - " << *o << "\n";

    for (size_t idx = 0; idx < lovituriData.size(); idx++) {
        float unghi = lovituriData[idx].first;
        float forta = lovituriData[idx].second;
        cout << "\n[Lovitura " << idx + 1 << "] unghi=" << unghi << " forta=" << forta << "\n";

        minge.loveste(forta, unghi);
        for (int t = 0; t < 300; t++) {
            Vector2D prev = minge.getPoz();
            minge.actualizeazaPozitia(0.1f);
            for (auto& o : obstacole)
                o->interact(minge, prev);
            if (gaura.contine(minge.getPoz())) {
                cout << ">> Minge in gaura dupa " << idx + 1 << " lovituri!\n";
                return true;
            }
            if (minge.vitezaMica()) break;
        }
        cout << minge << "\n";
    }

    cout << ">> Nivel pierdut.\n";
    return false;
}
