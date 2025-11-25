#include "BlackHole.h"
#include "WhiteHole.h"
#include <iostream>

BlackHole::BlackHole(const Vector2D& centru, float razaInfluenta, float razaAbsorbtie, float forta)
    : Obstacle(centru.getX() - razaInfluenta, centru.getX() + razaInfluenta,
               centru.getY() - razaInfluenta, centru.getY() + razaInfluenta),
      centru(centru),
      razaInfluenta(razaInfluenta),
      razaAbsorbtie(razaAbsorbtie),
      fortaGravitationala(forta) {}

void BlackHole::interact(Ball& b, const Vector2D& prev, float dt) {
    Vector2D poz = b.getPoz();
    Vector2D catreCentru = centru - poz;
    float dist = catreCentru.lungime();
    if (dist <= razaAbsorbtie) {
        Vector2D directieIntrare = (poz - prev).normalizat();
        if (directieIntrare.lungime() < 1e-4f) {
            directieIntrare = (catreCentru.normalizat()) * -1.0f;
        }
        if (auto exit = destinatie.lock()) {
            Vector2D iesire = exit->pozitieDeEmergenta(directieIntrare);
            b.seteazaPozitia(iesire);
            std::cout << ">> Gaura neagra a teleportat mingea!\n";
            exit->energizeaza(b, directieIntrare);
        } else {
            b.seteazaPozitia(centru);
        }
        return;
    }
    if (dist <= razaInfluenta && dist > 1e-4f) {
        float strength = fortaGravitationala / (dist * dist + 1.0f);
        Vector2D acceleratie = catreCentru.normalizat() * strength;
        b.aplicaAcceleratie(acceleratie, dt);
    }
}

std::string BlackHole::tip() const {
    return "Gaura Neagra";
}

void BlackHole::seteazaDestinatie(const std::shared_ptr<WhiteHole>& white) {
    destinatie = white;
}

const Vector2D& BlackHole::getCentru() const {
    return centru;


}

std::ostream& BlackHole::afisare(std::ostream& os) const {

    Obstacle::afisare(os);

    os << " {Centru: " << centru << ", Atractie: " << fortaGravitationala
       << ", Raza Abs: " << razaAbsorbtie << "}";
    return os;
}