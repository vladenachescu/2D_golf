#include "Level.h"
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>

// Aceste include-uri sunt necesare pentru dynamic_cast si logica din incarca
#include <ranges>

#include "BlackHole.h"
#include "WhiteHole.h"

Level::Level(const Ball& mingeInit, const Hole& gauraInit, std::vector<std::shared_ptr<Obstacle>> obstacoleInit)
    : minge(mingeInit), gaura(gauraInit), obstacole(std::move(obstacoleInit)) {}

void Level::incarca(int nrNivel, std::istream& in) {
    std::cout << "\n=== Configurare nivel " << nrNivel << " ===\n";
    float xStart, yStart;
    std::cout << "Pozitie start minge (x y): ";
    in >> xStart >> yStart;

    float hx, hy, hr;
    std::cout << "Pozitie si raza gaura (x y r): ";
    in >> hx >> hy >> hr;
    gaura = Hole(Vector2D(hx, hy), hr);

    int nrObs;
    std::cout << "Numar obstacole: ";
    in >> nrObs;
    obstacole.clear();

    std::unordered_map<int, std::shared_ptr<BlackHole>> gauriNegre;
    std::unordered_map<int, std::shared_ptr<WhiteHole>> gauriAlbe;
    std::vector<std::shared_ptr<WhiteHole>> listaGauriAlbe;

    int pairId;

    for (int i = 0; i < nrObs; i++) {

        std::string tip;
        std::cout << "  Obstacol " << i+1 << " (tip [WALL/WATER/SAND/BLACK_HOLE/WHITE_HOLE] si parametri): ";
        in >> tip;
        if (tip == "WALL") {
            float xmin, xmax, ymin, ymax;
            std::cout<< "xmin xmax yman ymax: ";
            in >> xmin >> xmax >> ymin >> ymax;
            obstacole.push_back(std::make_shared<Wall>(xmin, xmax, ymin, ymax));
        } else if (tip == "WATER") {
            float xmin, xmax, ymin, ymax;
            std::cout<< "xmin xmax yman ymax: ";
            in >> xmin >> xmax >> ymin >> ymax;
            obstacole.push_back(std::make_shared<Water>(xmin, xmax, ymin, ymax));
        } else if (tip == "SAND") {
            float xmin, xmax, ymin, ymax;
            std::cout<< "xmin xmax yman ymax: ";
            in >> xmin >> xmax >> ymin >> ymax;
            obstacole.push_back(std::make_shared<Sand>(xmin, xmax, ymin, ymax));

        } else if (tip == "BLACK_HOLE" || tip == "BLACKHOLE" || tip == "WHITE_HOLE" || tip == "WHITEWHOLE") {
            pairId = i;
            float cx, cy, razaInfluenta, razaAbsorbtie;
            std::cout<< "cx, cy, razaInfluenta, razaAbsorbtie \n";
            in >> cx >> cy >> razaInfluenta >> razaAbsorbtie;
            auto gh = std::make_shared<BlackHole>(Vector2D(cx, cy), razaInfluenta, razaAbsorbtie, pairId);
            obstacole.push_back(gh);
            gauriNegre[pairId] = gh;
            std::cout<< "daca ai una trebuie sa o ai si pe cealalata!\n\n";
            std::cout<< "cx cy razaRepulsie";
            float razaRepulsie;
            in >> cx >> cy >> razaRepulsie;
            auto wh = std::make_shared<WhiteHole>(Vector2D(cx, cy), razaRepulsie, pairId);
            obstacole.push_back(wh);
            gauriAlbe[pairId] = wh;
            listaGauriAlbe.push_back(wh);


        }

         else {
            std::cerr << "Tip de obstacol necunoscut: " << tip << "\n";
        }
    }

    // Legare gauri negre de albe
    for (const auto& [id, gh] : gauriNegre) {
        auto it = gauriAlbe.find(id);
        if (it != gauriAlbe.end()) {
            gh->seteazaDestinatie(it->second);
            it->second->seteazaSursa(gh);
        }
    }



    // Ajustare pozitie initiala daca e prea aproape de White Hole
    Vector2D pozInitiala(xStart, yStart);
    bool ajustata = false;
    for (const auto& wh : listaGauriAlbe) {
        float dist = pozInitiala.dist(wh->getCentru());
        if (dist < wh->getRazaRepulsie()) {
            Vector2D directie = (pozInitiala - wh->getCentru()).normalizat();
            if (directie.lungime() < 1e-4f) {
                directie = Vector2D(1.0f, 0.0f);
            }
            float buffer = 0.5f;
            pozInitiala = wh->getCentru() + directie * (wh->getRazaRepulsie() + buffer);
            ajustata = true;
        }
    }
    if (ajustata) {
        std::cout << "Pozitia initiala a fost ajustata pentru a evita zona de respingere a unei gauri albe.\n";
    }
    minge = Ball(pozInitiala);
    std::cout << "Pozitie initiala minge: " << minge << "\n";
}

bool Level::simuleaza(std::istream& in) {
    std::cout << "\n--- START NIVEL ---\n";
    std::cout << gaura << "\n";
    std::cout << "Obstacole:\n";

    bool areGauriWormhole = false;
    for (const auto& o : obstacole) {
        if (!areGauriWormhole &&
            (dynamic_cast<const BlackHole*>(o.get()) != nullptr ||
             dynamic_cast<const WhiteHole*>(o.get()) != nullptr)) {
            areGauriWormhole = true;
             }
        std::cout << "  - " << *o << "\n";
    }

    int lovituri = 0;

    while (true) {
        float unghi, forta;
        std::cout << "\n[Lovitura " << ++lovituri << "] Unghi si forta (grade, valoare) sau -1 pentru iesire: ";
        if (!(in >> unghi >> forta)) return false;
        if (unghi == -1 && forta == -1) return false;

        minge.loveste(forta, unghi);

        // Bucla fizica
        for (int t = 0; t < 300; t++) {
            constexpr float dt = 0.1f;
            Vector2D prev = minge.getPoz();
            minge.actualizeazaPozitia(dt);

            for (const auto& o : obstacole)
                o->interact(minge, prev, dt);

            if (gaura.contine(minge.getPoz())) {
                std::cout << ">> Minge in gaura dupa " << lovituri << " lovituri!\n";
                return true;
            }

            if (minge.vitezaMica()) break;
        }
        std::cout << minge << "\n";
    }
}
