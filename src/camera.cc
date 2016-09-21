#include <math.h>

#include "camera.h"

namespace mini3d {

Matrix Camera::GetMatrix() const {
    return GetPerspective() * GetView();
}

    Matrix Camera::GetPerspective() const {
        Matrix m;
        m.set_value(0, 0, 1 / tan(angle_ * 0.5));
        m.set_value(1, 1, 1 / tan(angle_ * 0.5) * ratio_);
        m.set_value(2, 2, (near_ + far_) / (far_ - near_));
        m.set_value(2, 3, (2 * far_ * near_) / (near_ - far_));
        m.set_value(3, 2, 1.0f);
        return m;
    }

    Matrix Camera::GetView() const {
        Matrix m = Matrix::IDENTITY();
        return m;
    }

void Camera::set_ratio(const F32& ratio) { ratio_ = ratio; }

const F32& Camera::ratio() const { return ratio_; }

}  // namespace mini3d
