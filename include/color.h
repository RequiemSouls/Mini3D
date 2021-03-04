#ifndef MINI3D_COLOR_H_
#define MINI3D_COLOR_H_

#include "types.h"
#include "config.h"

MINI_NS_BEGIN
struct Color {
    F32 r = 0;
    F32 g = 0;
    F32 b = 0;

    Color() = default;
    Color(F32 cr, F32 cg, F32 cb) {
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

    Color operator-(const Color &rc) {
        Color c;
        c.r = r - rc.r;
        c.g = g - rc.g;
        c.b = b - rc.b;
        return c;
    }

    Color operator+(const Color &rc) {
        Color c;
        c.r = r + rc.r;
        c.g = g + rc.g;
        c.b = b + rc.b;
        return c;
    }

    Color operator*(const F32 rhs) {
        Color c;
        c.r = r * rhs;
        c.g = g * rhs;
        c.b = b * rhs;
        return c;
    }

    void print() const {
        printf("c = {r: %f, g: %f, b: %f}\n", r, g, b);
    }

    static const Color ZERO;
    static const Color WHITE;
};

MINI_NS_END
#endif // MINI3D_COLOR_H_
