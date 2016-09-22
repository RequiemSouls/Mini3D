#ifndef MINI3D_GEOMETRY_H_
#define MINI3D_GEOMETRY_H_

#include "vector.h"
#include "matrix.h"
#include "color.h"
#include "types.h"

namespace mini3d {
#define PI 3.14159

typedef VectorImpl<F32, 2> Vec2;
typedef VectorImpl<F32, 4> Vector;
typedef MatrixImpl<F32, 4, 4> Matrix;


struct Vertex {
    Vector p;
    Color c;
};


}  // namespace mini3d

#endif  // MINI3D_GEOMETRY_H_
