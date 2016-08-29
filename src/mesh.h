#ifndef MINI3D_MESH_H_
#define MINI3D_MESH_H_

#include <vector>

#include "geometry.h"
#include "renderer.h"

namespace mini3d {

class Mesh {
public:
    static Mesh GenTriangle();
    static Mesh GenByFile(const char *fileName);

    void Draw(Renderer *r, const Matrix &m);

private:
    std::vector<I32> indices_;
    std::vector<Vertex> vertices_;
};

}  // namespace mini3d

#endif  // MINI3D_MESH_H_
