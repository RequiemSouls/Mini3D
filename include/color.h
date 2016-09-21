#ifndef MINI3D_COLOR_H_
#define MINI3D_COLOR_H_

#include "types.h"

namespace mini3d {

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

} // namespace mini3d
#endif // MINI3D_COLOR_H_