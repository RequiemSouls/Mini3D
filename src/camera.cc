#include <math.h>

#include "camera.h"

namespace mini3d {

Matrix Camera::GetMatrix() const {
    Matrix m;
    m.set_value(0, 0, 1 / tan(3.14 / 4));
    m.set_value(1, 1, 1 / tan(3.14 / 4) * ratio_);
    m.set_value(2, 2, (near_ + far_) / (far_ - near_));
    m.set_value(2, 3, (2 * far_ * near_) / (near_ - far_));
    m.set_value(3, 2, 1.0f);
    return m;
}

}  // namespace mini3d
