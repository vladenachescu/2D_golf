#pragma once
#include <iomanip>
using namespace std;

class Vector2D {
private:
    float x, y;
public:
    Vector2D(float x=0, float y=0);
    [[nodiscard]] float getX() const;
    [[nodiscard]] float getY() const;
    [[nodiscard]] float dist(const Vector2D& o) const;
    Vector2D operator+(const Vector2D& o) const;
    Vector2D operator*(float s) const;
    friend ostream& operator<<(ostream& os, const Vector2D& v);
};
