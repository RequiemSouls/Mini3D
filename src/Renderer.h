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
    I32 meshCount_ = 0;
    Color renderBuffer_[BUFFER_SIZE][BUFFER_SIZE];  // max screen 2048x2048
    Device &device_ = Device::GetInstance();
};

#endif