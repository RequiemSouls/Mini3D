#ifndef _MINI3D_RENDERER_H_
#define _MINI3D_RENDERER_H_

#include "mesh.h"
#include "device.h"

class Mesh;
class Renderer {
public:
    Renderer();
    ~Renderer();
    void DrawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3);
    void AddMesh(Mesh *mesh);
    void Render();
    void Buffer2Screen();

private:
    I16 width_ = 0;
    I16 height_ = 0;
    Matrix camera_ = Matrix::IDENTITY;
    Mesh **meshs_ = nullptr;
    I32 mesh_count_ = 0;
    Color **render_buffer_;
    Device &device_ = Device::GetInstance();
};

#endif