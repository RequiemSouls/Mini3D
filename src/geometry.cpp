#include "geometry.h"

const F32 identity[] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
    0.f, 0.f, 0.f, 0.f, 1.f};

const Vec2 Vec2::ZERO = Vec2();
const Vector Vector::ZERO = Vector();
const Color Color::ZERO = Color();

const Matrix Matrix::ZERO = Matrix();
const Matrix Matrix::IDENTITY((F32 *)(identity));