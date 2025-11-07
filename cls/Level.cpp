#include "Level.h"
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>

Level::Level(const Ball& mingeInit, const Hole& gauraInit, std::vector<std::shared_ptr<Obstacle>> obstacoleInit)
    : minge(mingeInit), gaura(gauraInit), obstacole(std::move(obstacoleInit)) {}

void Level::incarca(int nrNivel, std::istream& in) {
    std::cout << "\n=== Configurare nivel " << nrNivel << " ===\n";
    float xminStart, yminStart, xmaxStart, ymaxStart;
    std::cout << "Zona start minge (xmin ymin xmax ymax): ";
    in >> xminStart >> yminStart >> xmaxStart >> ymaxStart;
    if (xminStart > xmaxStart) std::swap(xminStart, xmaxStart);
    if (yminStart > ymaxStart) std::swap(yminStart, ymaxStart);
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

    for (int i = 0; i < nrObs; i++) {
        std::string tip;
        std::cout << "  Obstacol " << i+1 << " (tip WALL/WATER/SAND/BLACK_HOLE/WHITE_HOLE): ";
        in >> tip;
        if (tip == "WALL") {
            float xmin, xmax, ymin, ymax;
            std::cout << "\n xmin, ymin, xmax, ymax: ";
            in >> xmin >> ymin >> xmax >> ymax;
            obstacole.push_back(std::make_shared<Wall>(xmin, xmax, ymin, ymax));
        } else if (tip == "WATER") {
            float xmin, xmax, ymin, ymax;
            std::cout << "\n xmin, ymin, xmax, ymax: ";
            in >> xmin >> ymin >> xmax >> ymax;
            obstacole.push_back(std::make_shared<Water>(xmin, xmax, ymin, ymax));
        } else if (tip == "SAND") {
            float xmin, xmax, ymin, ymax;
            std::cout << "\n xmin, ymin, xmax, ymax: ";
            in >> xmin >> ymin >> xmax >> ymax;
            obstacole.push_back(std::make_shared<Sand>(xmin, xmax, ymin, ymax));
        } else if (tip == "BLACK_HOLE" || tip == "BLACKHOLE") {
            float cx, cy, razaInfluenta, razaAbsorbtie;
            int pairId;
            std::cout << "\n cx, cy , razaInfluenta, razaAbsorbtie , pairId: ";
            in >> cx >> cy >> razaInfluenta >> razaAbsorbtie >> pairId;
            auto gh = std::make_shared<BlackHole>(Vector2D(cx, cy), razaInfluenta, razaAbsorbtie);
            obstacole.push_back(gh);
            gauriNegre[pairId] = gh;
        } else if (tip == "WHITE_HOLE" || tip == "WHITEHOLE") {
            float cx, cy, razaRepulsie;
            int pairId;
            in >> cx >> cy >> razaRepulsie >> pairId;
            std::cout<< "\n cx, cy , razaRepulsie, pairId : ";
            auto wh = std::make_shared<WhiteHole>(Vector2D(cx, cy), razaRepulsie);
            obstacole.push_back(wh);
            gauriAlbe[pairId] = wh;
            listaGauriAlbe.push_back(wh);
        } else {
            std::cerr << "Tip de obstacol necunoscut: " << tip << "\n";
        }
    }
    for (const auto& [id, gh] : gauriNegre) {
        auto it = gauriAlbe.find(id);
        if (it != gauriAlbe.end()) {
            gh->seteazaDestinatie(it->second);
            it->second->seteazaSursa(gh);
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(xminStart, xmaxStart);
    std::uniform_real_distribution<float> distY(yminStart, ymaxStart);
    Vector2D pozInitiala(xminStart, yminStart);
    bool gasit = false;
    for (int incercari = 0; incercari < 200; ++incercari) {
        Vector2D candidat(distX(gen), distY(gen));
        bool inZonaRepulsie = false;
        for (const auto& wh : listaGauriAlbe) {
            if (candidat.dist(wh->getCentru()) <= wh->getRazaRepulsie()) {
                inZonaRepulsie = true;
                break;
            }
        }
        if (!inZonaRepulsie) {
            pozInitiala = candidat;
            gasit = true;
            break;
        }
    }
    if (!gasit) {
        pozInitiala = Vector2D((xminStart + xmaxStart)/2.0f, (yminStart + ymaxStart)/2.0f);
    }
    minge = Ball(pozInitiala);
    std::cout << "Pozitie initiala minge: " << minge << "\n";
}

bool Level::simuleaza(std::istream& in) {
    std::cout << "\n--- START NIVEL ---\n";
    std::cout << gaura << "\n";
    std::cout << "Obstacole:\n";
    for (const auto& o : obstacole) std::cout << "  - " << *o << "\n";

    int lovituri = 0;

    while (true) {
        float unghi, forta;
        std::cout << "\n[Lovitura " << ++lovituri << "] Unghi si forta (grade, valoare) sau -1 pentru iesire: ";
        if (!(in >> unghi >> forta)) return false;
        if (unghi == -1 && forta == -1) return false;

        minge.loveste(forta, unghi);
        const float dt = 0.1f;
        for (int t = 0; t < 300; t++) {
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
std::ostream& operator<<(std::ostream& os, const Level& level) {
    os << "Level cu gaura " << level.gaura << " si " << level.obstacole.size()
       << " obstacole";
    return os;
}