#ifndef _MINI3D_GEOMETRY_H_
#define _MINI3D_GEOMETRY_H_

struct Vector {
    float x;
    float y;
    float z;
    float w;

    Vector() {
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }
};

struct Matrix {
    float m[4][4];

    static Matrix identity() {
        return Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    }
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color(unsigned char vr, unsigned char vg, unsigned char vb) {
        r = vr;
        g = vg;
        b = vb;
    }

    Color() {
        r = g = b = 0;
    }
};

struct Vertex{
    Vector p;
    Color c;
};
#endif