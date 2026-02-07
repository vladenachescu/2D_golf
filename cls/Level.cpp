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



Level::Level(const Ball& mingeInit, const Hole& gauraInit, std::vector<std::shared_ptr<Obstacle>> obstacoleInit)
    : minge(mingeInit), startPoz(mingeInit.getPoz()), gaura(gauraInit), obstacole(std::move(obstacoleInit)) {}

// [NEW] Copy Constructor (Deep Copy)
Level::Level(const Level& other) 
    : minge(other.minge), startPoz(other.startPoz), gaura(other.gaura) {
    obstacole.reserve(other.obstacole.size());
    for (const auto& obs : other.obstacole) {
        obstacole.push_back(obs->clone());
    }
}

// [NEW] Assignment Operator (Copy-and-Swap idiom implies we could use a swap, but standard assign is fine here)
Level& Level::operator=(const Level& other) {
    if (this != &other) {
        minge = other.minge;
        startPoz = other.startPoz;
        gaura = other.gaura;
        obstacole.clear();
        obstacole.reserve(other.obstacole.size());
        for (const auto& obs : other.obstacole) {
            obstacole.push_back(obs->clone());
        }
    }
    return *this;
}

void Level::incarca(int nrNivel, std::istream& in) {
    std::cout << "\n=== Configurare nivel " << nrNivel << " ===\n";
    float xStart, yStart;
    std::cout << "Pozitie start minge (x y): ";
    in >> xStart >> yStart;
    minge = Ball(Vector2D(xStart, yStart));
    startPoz = Vector2D(xStart, yStart);

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


void Level::update(float dt) {
    Vector2D prev = minge.getPoz();
    minge.actualizeazaPozitia(dt);
    
    // World Boundary Check (0-80, 0-60)
    Vector2D pos = minge.getPoz();
    float r = minge.getRaza();
    bool hit = false;
    
    // Left
    if (pos.getX() - r < 0.0f) {
        minge.setPoz(Vector2D(r + 0.01f, pos.getY()));
        minge.reflecta(Vector2D(1, 0));
        hit = true;
    }
    // Right
    else if (pos.getX() + r > 80.0f) {
        minge.setPoz(Vector2D(80.0f - r - 0.01f, pos.getY()));
        minge.reflecta(Vector2D(-1, 0));
        hit = true;
    }
    
    // Top
    if (pos.getY() - r < 0.0f) {
         minge.setPoz(Vector2D(pos.getX(), r + 0.01f));
         minge.reflecta(Vector2D(0, 1));
         hit = true;
    }
    // Bottom
    else if (pos.getY() + r > 60.0f) {
        minge.setPoz(Vector2D(pos.getX(), 60.0f - r - 0.01f));
        minge.reflecta(Vector2D(0, -1));
        hit = true;
    }
    
    if (hit) std::cout << ">> Loveste marginea hartii!\n";

    for (const auto& o : obstacole)
        o->interact(minge, prev, dt);
}

void Level::render(sf::RenderTarget& target) {
    // Draw obstacles
    for (const auto& o : obstacole) {
        o->render(target);
    }
    // Draw Hole
    gaura.render(target);
    // Draw Ball
    minge.render(target);
}

bool Level::isFinished() const {
    return gaura.contine(minge.getPoz());
}

/* 
// Deprecated console simulation
bool Level::simuleaza(std::istream& in) {
    // ... (original content commented out or removed)
    return false;
} 
*/


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


void Level::reseteazaMinge() {
    minge.seteazaPozitia(startPoz);
}

Ball& Level::getMinge() { return minge; }