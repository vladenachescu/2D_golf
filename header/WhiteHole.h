#pragma once
#include "Obstacle.h"
#include <memory>
#include <string>

class BlackHole;

class WhiteHole : public Obstacle {
private:
    Vector2D centru;
    float razaRepulsie;
    float fortaRepulsie;
    std::weak_ptr<BlackHole> sursa;
public:
    WhiteHole(const Vector2D& centru, float razaRepulsie, float forta=35.0f);
    void interact(Ball& b, const Vector2D& prev, float dt) override;
    [[nodiscard]] std::string tip() const override;
    void seteazaSursa(const std::shared_ptr<BlackHole>& black);
    [[nodiscard]] Vector2D pozitieDeEmergenta(const Vector2D& directiePreferata) const;
    void energizeaza(Ball& b, const Vector2D& directieIntrare) const;
    [[nodiscard]] const Vector2D& getCentru() const;
    [[nodiscard]] float getRazaRepulsie() const;
    std::ostream& afisare(std::ostream& os) const override;
};