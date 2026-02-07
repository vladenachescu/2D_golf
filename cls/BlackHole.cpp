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
        float strength = fortaGravitationala / (dist * dist+2);
        Vector2D acceleratie = catreCentru.normalizat() * strength;
        b.aplicaAcceleratie(acceleratie, dt);
    }
}

std::string BlackHole::tip() const {
    return "Gaura Neagra";
}

void BlackHole::render(sf::RenderTarget& target) const {
    sf::CircleShape circle(razaInfluenta * 10.0f);
    circle.setFillColor(sf::Color(20, 0, 20)); // Dark purple/black
    circle.setOutlineColor(sf::Color::Magenta);
    circle.setOutlineThickness(2.0f);
    circle.setOrigin({razaInfluenta * 10.0f, razaInfluenta * 10.0f});
    circle.setPosition({centru.getX() * 10.0f, centru.getY() * 10.0f});
    target.draw(circle);
    
    // Inner absorption zone
    sf::CircleShape inner(razaAbsorbtie * 10.0f);
    inner.setFillColor(sf::Color::Black);
    inner.setOutlineColor(sf::Color::White); // High contrast for absorption zone
    inner.setOutlineThickness(1.0f);
    inner.setOrigin({razaAbsorbtie * 10.0f, razaAbsorbtie * 10.0f});
    inner.setPosition({centru.getX() * 10.0f, centru.getY() * 10.0f});
    target.draw(inner);
}

void BlackHole::seteazaDestinatie(const std::shared_ptr<WhiteHole>& white) {
    destinatie = white;
}


std::ostream& BlackHole::afisare(std::ostream& os) const {

    Obstacle::afisare(os);

    os << " {Centru: " << centru << ", Atractie: " << fortaGravitationala
       << ", Raza Abs: " << razaAbsorbtie << "}";
    return os;
}