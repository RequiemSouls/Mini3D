#ifndef _MINI3D_MESH_H_
#define _MINI3D_MESH_H_

#include "Renderer.h"
#include "geometry.h"
#include "Types.h"

class Renderer;
class Mesh {
public:
    Mesh();
    ~Mesh();
    static Mesh *genTriangle();
    static Mesh *genByFile(const char *fileName);
    void draw(Renderer *r, Matrix m);

private:
    I32 indexArray[256] = {0};
    Vertex **vertexArray = nullptr;
    I32 tCount = 0;
};

#endif