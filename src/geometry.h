#ifndef MINI3D_GEOMETRY_H_
#define MINI3D_GEOMETRY_H_

#include <math.h>
#include <string.h>
#include "types.h"

namespace mini3d {

struct Vec2 {
    F32 x = 0;
    F32 y = 0;

    Vec2() = default;
    Vec2(F32 px, F32 py) : x(px), y(py) {}

    Vec2 operator-(const Vec2 &other) const { return Vec2(x - other.x, y - other.y); }

    Vec2 operator+(const Vec2 &other) const { return Vec2(x + other.x, y + other.y); }

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

    F32 Length() const { return sqrt(x * x + y * y); }

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

    Vector() = default;
    Vector(F32 vx, F32 vy, F32 vz, F32 vw) {
        x = vx;
        y = vy;
        z = vz;
        w = vw;
    }
    Vector(const Vector &value) {
        x = value.x;
        y = value.y;
        z = value.z;
        w = value.w;
    }

    void Homogenize() {
        x /= w;
        y /= w;
        z /= w;
        w = 1.0f;
    }

    static const Vector ZERO;
};

struct Matrix {
    F32 m[4][4] = {{0}, {0}, {0}, {0}};

    Matrix() = default;
    explicit Matrix(const F32 *mat) { memcpy(m, mat, sizeof(m)); }
    Matrix operator*(const Matrix &m1) const {
        Matrix r;
        for (I32 i = 0; i < 4; i++) {
            for (I32 j = 0; j < 4; j++) {
                r.m[i][j] = m[i][0] * m1.m[0][j] + m[i][1] * m1.m[1][j] + m[i][2] * m1.m[2][j] + m[i][3] * m1.m[3][j];
            }
        }
        return r;
    }
    void Transfer(const Vector &offset);

    static const Matrix ZERO;
    static const Matrix IDENTITY;
};

struct Color {
    UI8 r = 0;
    UI8 g = 0;
    UI8 b = 0;

    Color() = default;
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
    static const Color WHITE;
};

struct Vertex {
    Vector p;
    Color c;
};

Vector operator*(const Matrix &m, const Vector &v);

}  // namespace mini3d

#endif  // MINI3D_GEOMETRY_H_
