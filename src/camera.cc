#include <math.h>

#include "camera.h"

namespace mini3d {

Matrix Camera::GetMatrix() const {
    Matrix m;
    m.m[0][0] = 1 / tan(3.14 / 4);
    m.m[1][1] = 1 / tan(3.14 / 4) * ratio_;
    m.m[2][2] = (near_ + far_) / (far_ - near_);
    m.m[2][3] = (2 * far_ * near_) / (near_ - far_);
    m.m[3][2] = 1.0f;
    return m;
}

}  // namespace mini3d
