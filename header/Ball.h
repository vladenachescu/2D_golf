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
    Ball(const Ball& other);
    Ball& operator=(const Ball& other);
    ~Ball();
    void loveste(float forta, float unghi);
    void actualizeazaPozitia(float dt);
    void incetineste(float factor);
    void reflectaX();
    void reflectaY();
    void reseteaza();
    void aplicaAcceleratie(const Vector2D& acceleratie, float dt);
    void seteazaPozitia(const Vector2D& p);
    void adaugaViteza(const Vector2D& delta);
    const Vector2D& getPoz() const;
    const Vector2D& getUltimaPoz() const;
    bool vitezaMica() const;
    friend std::ostream& operator<<(std::ostream& os, const Ball& b);
};
