#ifndef MINI3D_MESH_H_
#define MINI3D_MESH_H_

#include "geometry.h"
#include "renderer.h"
#include <vector>

namespace mini3d {

class Mesh {
public:
    Mesh();
    ~Mesh();
    static Mesh *GenTriangle();
    static Mesh *GenByFile(const char *fileName);
    void Draw(Renderer *r, Matrix &m);

private:
    std::vector<I32> indices_;
    std::vector<Vertex> vertices_;
};

}  // namespace mini3d

#endif  // MINI3D_MESH_H_
