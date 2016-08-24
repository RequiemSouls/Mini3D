#ifndef MINI3D_GEOMETRY_H_
#define MINI3D_GEOMETRY_H_

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

namespace mini3d {

struct Vec2 {
    F32 x = 0;
    F32 y = 0;

    Vec2() {}
    Vec2(F32 px, F32 py) : x(px), y(py) {}

    Vec2 operator-(const Vec2 &other) { return Vec2(x - other.x, y - other.y); }

    Vec2 operator+(const Vec2 &other) { return Vec2(x + other.x, y + other.y); }

    Vec2 &operator+=(const Vec2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2 &operator-=(const Vec2 &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    F32 Length() { return sqrt(x * x + y * y); }

    void Normalized() {
        F32 len = sqrt(x * x + y * y);
        x /= len;
        y /= len;
    }

    static const Vec2 ZERO;
};

struct Vector {
    F32 x = 0.f;
    F32 y = 0.f;
    F32 z = 0.f;
    F32 w = 1.f;

    Vector() {}

    static const Vector ZERO;
};

struct Matrix {
    F32 m[4][4] = {{0}, {0}, {0}, {0}};

    Matrix() {}
    Matrix(F32 *mat) { memcpy(m, mat, 16); }

    static const Matrix ZERO;
    static const Matrix IDENTITY;
};

struct Color {
    UI8 r = 0;
    UI8 g = 0;
    UI8 b = 0;

    Color() {}
    Color(UI8 cr, UI8 cg, UI8 cb) {
        r = cr;
        g = cg;
        b = cb;
    }

    bool operator==(const Color &other) const {
        return (r == other.r) && (g == other.g) && (b == other.b);
    }

    bool operator!=(const Color &other) const {
        return (r != other.r) || (g != other.g) || (b != other.b);
    }

    static const Color ZERO;
};

struct Vertex {
    Vector p;
    Color c;
};

}  // namespace mini3d

#endif  // MINI3D_GEOMETRY_H_
