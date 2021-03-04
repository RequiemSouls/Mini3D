#ifndef MINI3D_GEOMETRY_H_
#define MINI3D_GEOMETRY_H_

#include "vector.h"
#include "matrix.h"
#include "color.h"
#include "types.h"
#include "config.h"

MINI_NS_BEGIN
#define PI 3.14159f

typedef VectorImpl<F32, 2> Vec2;
typedef VectorImpl<F32, 4> Vector;
typedef MatrixImpl<F32, 4, 4> Matrix;


struct Vertex {
    Vector p;
    Color c;

    void print() const {
        p.print();
        c.print();
    }
    Vertex operator-(const Vertex &rvt) {
    	Vertex vt;
    	vt.c = c - rvt.c;
    	vt.p = p - rvt.p;
    	return vt;
    }
    Vertex operator+(const Vertex &rvt) {
    	Vertex vt;
    	vt.c = c + rvt.c;
    	vt.p = p + rvt.p;
    	return vt;
    }
    Vertex operator*(const F32 lhs) {
    	Vertex vt;
    	vt.c = c * lhs;
    	vt.p = p * lhs;
    	return vt;
    }
  };

MINI_NS_END

#endif  // MINI3D_GEOMETRY_H_
