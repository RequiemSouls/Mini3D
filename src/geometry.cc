#include "geometry.h"

namespace mini3d {

// const F32 identity[] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
//                         0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

// const Vec2 Vec2::ZERO = Vec2();
// const Vector Vector::ZERO = Vector();
const Color Color::ZERO = Color();
const Color Color::WHITE = Color(255, 255, 255);

// const Matrix Matrix::ZERO = Matrix();
// const Matrix Matrix::IDENTITY((F32 *)(identity));
//
// void Matrix::Transfer(const Vector &offset) {
//     m[0][3] = offset.get_x();
//     m[1][3] = offset.get_y();
//     m[2][3] = offset.get_z();
// }
//
// Vector operator*(const Matrix &m, const Vector &v) {
//     return Vector(
//         m.m[0][0] * v.get_x() + m.m[0][1] * v.get_y() + m.m[0][2] * v.get_z() + m.m[0][3] * v.get_w(),
//         m.m[1][0] * v.get_x() + m.m[1][1] * v.get_y() + m.m[1][2] * v.get_z() + m.m[1][3] * v.get_w(),
//         m.m[2][0] * v.get_x() + m.m[2][1] * v.get_y() + m.m[2][2] * v.get_z() + m.m[2][3] * v.get_w(),
//         m.m[3][0] * v.get_x() + m.m[3][1] * v.get_y() + m.m[3][2] * v.get_z() + m.m[3][3] * v.get_w());
// }

}  // namespace mini3d
