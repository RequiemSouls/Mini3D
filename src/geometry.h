#ifndef _MINI3D_GEOMETRY_H_
#define _MINI3D_GEOMETRY_H_

struct Vector {
    float x = 0;
    float y = 0;
    float z;
    float w;
};

struct Matrix {
    float m[4][4];
};

struct Color {
    char r;
    char g;
    char b;
};

struct Vertex{
    Vector p;
    Color c;
};
#endif