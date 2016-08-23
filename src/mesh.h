#ifndef _MINI3D_MESH_H_
#define _MINI3D_MESH_H_

#include "renderer.h"
#include "geometry.h"
#include "types.h"

class Renderer;
class Mesh {
public:
    Mesh();
    ~Mesh();
    static Mesh *GenTriangle();
    static Mesh *GenByFile(const char *fileName);
    void Draw(Renderer *r, Matrix m);

private:
    I32 index_array_[256] = {0};
    Vertex **vertex_array_ = nullptr;
    I32 count_ = 0;
};

#endif