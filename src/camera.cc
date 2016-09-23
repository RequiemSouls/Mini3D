#include <math.h>

#include "vector.h"
#include "camera.h"

namespace mini3d {

Matrix Camera::GetMatrix() const {
    return GetPerspective() * GetView();
}

Matrix Camera::GetPerspective() const {
    Matrix m;
    F32 fax = 1.0f / (float)tan(angle_ * 0.5f);
    m.set_value(0, 0, fax);
    m.set_value(1, 1, fax / ratio_);
    m.set_value(2, 2, (near_ + far_) / (far_ - near_));
    m.set_value(2, 3, (2 * far_ * near_) / (near_ - far_));
    m.set_value(3, 2, 1.0f);
    return m;
}

Matrix Camera::GetView() const {
    Matrix m = Matrix::IDENTITY();
    Vector world_up {0, 1, 0, 0};
    Vector zaxis = lookat_ - pos_;
    zaxis.Normalize();
    Vector xaxis = CrossProduct(world_up, zaxis);
    xaxis.Normalize();
    Vector yaxis = CrossProduct(zaxis, xaxis);
    // yaxis.Normalize();

    m.set_value(0, 0, xaxis.get_x());
    m.set_value(0, 1, xaxis.get_y());
    m.set_value(0, 2, xaxis.get_z());
    m.set_value(0, 3, DotProduct(xaxis, pos_));

    m.set_value(1, 0, yaxis.get_x());
    m.set_value(1, 1, yaxis.get_y());
    m.set_value(1, 2, yaxis.get_z());
    m.set_value(1, 3, DotProduct(yaxis, pos_));

    m.set_value(2, 0, zaxis.get_x());
    m.set_value(2, 1, zaxis.get_y());
    m.set_value(2, 2, zaxis.get_z());
    m.set_value(2, 3, DotProduct(zaxis, pos_));

    m.set_value(3, 0, 0);
    m.set_value(3, 1, 0);
    m.set_value(3, 2, 0);
    m.set_value(3, 3, 1);
    return m;
}

void Camera::set_lookat(const Vector& lookat) { lookat_ = lookat; }

const Vector& Camera::lookat() const { return lookat_; }

void Camera::set_ratio(const F32& ratio) { ratio_ = ratio; }

const F32& Camera::ratio() const { return ratio_; }

}  // namespace mini3d
