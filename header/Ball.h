#pragma once
#include "Vector2D.h"
#include <SFML/Graphics.hpp>

class Ball {
private:
    Vector2D pozitie;
    Vector2D viteza;
    Vector2D ultimaPozitie;
    [[maybe_unused]] float raza;
public:
    Ball();

    explicit Ball(const Vector2D& p, float r=1.0f);
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
    void seteazaPozitia(const Vector2D& p); // Resets velocity
    void setPoz(const Vector2D& p);         // Keeps velocity
    void reflecta(const Vector2D& normal);  // Vector reflection
    void adaugaViteza(const Vector2D& delta);
    [[nodiscard]] const Vector2D& getPoz() const;
    [[nodiscard]] const Vector2D& getUltimaPoz() const;
    [[nodiscard]] float getRaza() const;
    [[nodiscard]] bool vitezaMica() const;
    void render(sf::RenderTarget& target) const;
    void setTexture(const sf::Texture* tex) { m_texture = tex; }
    
    friend std::ostream& operator<<(std::ostream& os, const Ball& b);

private:
   const sf::Texture* m_texture = nullptr;
};
