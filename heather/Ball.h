#pragma once
#include "Vector2D.h"

class Ball {
private:
    Vector2D pozitie;
    Vector2D viteza;
    Vector2D ultimaPozitie;
    [[maybe_unused]] float raza;
public:
    Ball();

    explicit Ball(const Vector2D& p, float r=0.2f);
    void loveste(float forta, float unghi);
    void actualizeazaPozitia(float dt);
    void incetineste(float factor);
    void reflectaX();
    void reflectaY();
    void reseteaza();
    const Vector2D& getPoz() const;
    const Vector2D& getUltimaPoz() const;
    bool vitezaMica() const;
    friend ostream& operator<<(ostream& os, const Ball& b);
};
