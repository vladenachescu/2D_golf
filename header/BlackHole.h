#pragma once
#include "Obstacle.h"
#include <memory>
#include <string>

class WhiteHole;

class BlackHole : public Obstacle {
private:
    Vector2D centru;
    float razaInfluenta;
    float razaAbsorbtie;
    float fortaGravitationala;
    std::weak_ptr<WhiteHole> destinatie;
public:
    BlackHole(const Vector2D& centru, float razaInfluenta, float razaAbsorbtie, float forta=30.0f);
    void interact(Ball& b, const Vector2D& prev, float dt) override;
    [[nodiscard]] std::string tip() const override;
    void seteazaDestinatie(const std::shared_ptr<WhiteHole>& white);
    [[nodiscard]] const Vector2D& getCentru() const;
};