#include "stdio.h"
#include <math.h>
#include "camera.h"

namespace mini3d {

Camera::Camera() {
    near_ = 500.0f;
    far_ = 1000.0f;
    lookup_ = Vector(0.0f, 0.0f, 1.0f, 0.0f);
    pos_ = Vector(0.0f, 0.0f, 0.0f, 1.0f);
    angle_ = 90.0f;
    ratio_ = 1.0f;
}

Camera::~Camera() {
}

Matrix Camera::GetMatrix() {
    Matrix m;
    m.m[0][0] = 1/tan(3.14/4);
    m.m[1][1] = 1/tan(3.14/4) * ratio_;
    m.m[2][2] = (near_ + far_) / (far_ - near_);
    m.m[2][3] = (2 * far_ * near_) / (near_ - far_);
    m.m[3][2] = 1.0f;
    return m;
}

}