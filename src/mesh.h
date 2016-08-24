#ifndef MINI3D_MESH_H_
#define MINI3D_MESH_H_

#include "renderer.h"
#include "geometry.h"
#include "types.h"

namespace mini3d {

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

}  // namespace mini3d

#endif  // MINI3D_MESH_H_
