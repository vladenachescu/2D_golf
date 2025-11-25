#include "Level.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "BlackHole.h"
#include "WhiteHole.h"
#include "Wall.h"
#include "Water.h"
#include "Sand.h"

// Constructor
Level::Level(const Ball& mingeInit, const Hole& gauraInit, std::vector<std::shared_ptr<Obstacle>> obstacoleInit)
    : minge(mingeInit), gaura(gauraInit), obstacole(std::move(obstacoleInit)) {}

void Level::incarca(int nrNivel, std::istream& in) {
    std::cout << "\n=== Configurare nivel " << nrNivel << " ===\n";
    float xStart, yStart;
    std::cout << "Pozitie start minge (x y): ";
    in >> xStart >> yStart;
    minge = Ball(Vector2D(xStart, yStart));
    float hx, hy, hr;
    std::cout << "Pozitie si raza gaura (x y r): ";
    in >> hx >> hy >> hr;
    gaura = Hole(Vector2D(hx, hy), hr);
    int nrObs;
    std::cout << "Numar obstacole: ";
    in >> nrObs;
    obstacole.clear();
    for (int i = 0; i < nrObs; i++) {
        std::string tip;
        std::cout << "  Obstacol " << i+1 << " (tip [WALL/WATER/SAND/WORM_HOLES]: ";
        in >> tip;
        std::shared_ptr<Obstacle> nouObstacol = nullptr;
        if (tip == "WALL") {
            float xmin, xmax, ymin, ymax;
            std::cout << "xmin xmax ymin ymax: ";
            in >> xmin >> xmax >> ymin >> ymax;
            nouObstacol = std::make_shared<Wall>(xmin, xmax, ymin, ymax);
        }
        else if (tip == "WATER") {
            float xmin, xmax, ymin, ymax;
            std::cout << "xmin xmax ymin ymax: ";
            in >> xmin >> xmax >> ymin >> ymax;
            nouObstacol = std::make_shared<Water>(xmin, xmax, ymin, ymax);
        }
        else if (tip == "SAND") {
            float xmin, xmax, ymin, ymax;
            std::cout << "xmin xmax ymin ymax: ";
            in >> xmin >> xmax >> ymin >> ymax;
            nouObstacol = std::make_shared<Sand>(xmin, xmax, ymin, ymax);
        }
        else if (tip == "WORM_HOLES") {
            float cx, cy, rInf, rAbs;
            std::cout << "Gaura Neagra (cx cy razaInfluenta razaAbsorbtie): ";
            in >> cx >> cy >> rInf >> rAbs;
            auto gh = std::make_shared<BlackHole>(Vector2D(cx, cy), rInf, rAbs, 30.0f);
            float wcx, wcy, wRep;
            std::cout << "Gaura Alba Pereche (cx cy razaRepulsie): ";
            in >> wcx >> wcy >> wRep;
            auto wh = std::make_shared<WhiteHole>(Vector2D(wcx, wcy), wRep);
            if (gh->intersecteaza(minge.getPoz()) || wh->intersecteaza(minge.getPoz())) {
                std::cout << ">> EROARE: Nu poti pune o gaura peste minge! Pozitia mingii: " << minge.getPoz() << "\n";
                i--;
                continue;
            }
            gh->seteazaDestinatie(wh);
            wh->seteazaSursa(gh);
            obstacole.push_back(gh);
            obstacole.push_back(wh);
            continue;
        }
        else {
            std::cerr << "Tip de obstacol necunoscut: " << tip << "\n";
            i--;
            continue;
        }

        if (nouObstacol != nullptr) {
            if (nouObstacol->intersecteaza(minge.getPoz())) {
                std::cout << ">> EROARE: Obstacolul se suprapune cu mingea (" << minge.getPoz() << ")! Reintrodu coordonatele.\n";
                i--;
                continue;
            }
            obstacole.push_back(nouObstacol);
        }
    }

}

bool Level::simuleaza(std::istream& in) {

    std::cout << *this;

    int lovituri = 0;

    while (true) {
        float unghi, forta;
        std::cout << "\n[Lovitura " << ++lovituri << "] Unghi si forta (grade, valoare) sau -1 -1 pt iesire: ";
        if (!(in >> unghi >> forta)) return false;
        if (unghi == -1 && forta == -1) return false;

        minge.loveste(forta, unghi);

        bool miscare = true;
        int pasi = 0;
        while (miscare && pasi < 500) {
            constexpr float dt = 0.1f;
            Vector2D prev = minge.getPoz();
            minge.actualizeazaPozitia(dt);

            for (const auto& o : obstacole)
                o->interact(minge, prev, dt);

            if (gaura.contine(minge.getPoz())) {
                std::cout << ">> GOL! Minge in gaura dupa " << lovituri << " lovituri!\n";
                return true;
            }

            if (minge.vitezaMica()) miscare = false;
            pasi++;
        }
        std::cout << minge << "\n";
    }
}

void Level::adaugaObstacol(const std::shared_ptr<Obstacle>& obs) {
    obstacole.push_back(obs);
}

void Level::reseteazaMinge() {
    minge.reseteaza();
}

Ball& Level::getMinge() {
    return minge;
}

std::ostream& operator<<(std::ostream& os, const Level& level) {
    os << "=== STARE NIVEL ===\n";

    os << "Minge: " << level.minge << "\n";

    os << "Tinta: " << level.gaura << "\n";

    os << "Obstacole (" << level.obstacole.size() << "):\n";
    for (const auto& obs : level.obstacole) {
        os << "  -> " << *obs << "\n";
    }

    return os;
}