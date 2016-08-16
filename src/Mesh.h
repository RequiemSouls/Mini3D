#ifndef _MINI3D_MESH_H_
#define _MINI3D_MESH_H_

#include "geometry.h"
#include "Renderer.h"

class Mesh {
public:
    Mesh();
    ~Mesh();
    static Mesh* genTriangle();
    static Mesh* genByFile(const char* fileName);
    void draw(Renderer* r, Matrix m);

private:
    int indexArray[256];
    Vertex** vertexArray;
    int tCount;
};

#endif