#ifndef MINI3D_MESH_H_
#define MINI3D_MESH_H_

#include <vector>

#include "geometry.h"
#include "renderer.h"
#include "config.h"

MINI_NS_BEGIN

class Mesh {
public:
    static Mesh GenTriangle();
    static Mesh GenByFile(const char *fileName);

    void Draw(Renderer *r, const Matrix &m);

private:
    std::vector<I32> indices_;
    std::vector<Vertex> vertices_;
};

MINI_NS_END

#endif  // MINI3D_MESH_H_
