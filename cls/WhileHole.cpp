#include "WhiteHole.h"
#include "BlackHole.h"
#include <iostream>

WhiteHole::WhiteHole(const Vector2D& centru, float razaRepulsie, float forta)
    : Obstacle(centru.getX() - razaRepulsie, centru.getX() + razaRepulsie,
               centru.getY() - razaRepulsie, centru.getY() + razaRepulsie),
      centru(centru),
      razaRepulsie(razaRepulsie),
      fortaRepulsie(forta) {}

void WhiteHole::interact(Ball& b, const Vector2D&, float dt) {
    Vector2D poz = b.getPoz();
    Vector2D catreExterior = poz - centru;
    float dist = catreExterior.lungime();
    if (dist <= razaRepulsie && dist > 1e-4f) {
        float strength = fortaRepulsie / (dist * dist + 1.0f);
        Vector2D acceleratie = catreExterior.normalizat() * strength;
        b.aplicaAcceleratie(acceleratie, dt);
    }
}

std::string WhiteHole::tip() const {
    return "Gaura Alba";
}

void WhiteHole::seteazaSursa(const std::shared_ptr<BlackHole>& black) {
    sursa = black;
}

Vector2D WhiteHole::pozitieDeEmergenta(const Vector2D& directiePreferata) const {
    Vector2D dir = directiePreferata.normalizat();
    if (dir.lungime() < 1e-4f) {
        dir = Vector2D(1,0);
    }
    float buffer = 0.5f;
    return centru + dir * (razaRepulsie + buffer);
}

void WhiteHole::energizeaza(Ball& b, const Vector2D& directieIntrare) const {
    Vector2D dir = directieIntrare.normalizat();
    if (dir.lungime() < 1e-4f) {
        dir = Vector2D(1,0);
    }
    float impuls = fortaRepulsie * 0.1f;
    b.adaugaViteza(dir * impuls);
    std::cout << ">> Gaura alba respinge mingea!\n";
}

const Vector2D& WhiteHole::getCentru() const {
    return centru;
}

float WhiteHole::getRazaRepulsie() const {
    return razaRepulsie;
}
std::ostream& WhiteHole::afisare(std::ostream& os) const {
    Obstacle::afisare(os);
    os << " {Centru: " << centru << ", Respingere: " << fortaRepulsie << "}";
    return os;
}