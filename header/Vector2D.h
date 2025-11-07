#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

class Vector2D {
private:
    float x, y;
public:
    explicit Vector2D(float x=0, float y=0);
    float getX() const;
    float getY() const;
    float dist(const Vector2D& o) const;
    float lungime() const;
    Vector2D normalizat() const;
    Vector2D operator+(const Vector2D& o) const;
    Vector2D operator-(const Vector2D& o) const;
    Vector2D operator*(float s) const;
    Vector2D operator/(float s) const;
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
};
