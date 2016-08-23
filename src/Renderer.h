#ifndef _MINI3D_RENDERER_H_
#define _MINI3D_RENDERER_H_

#include "Mesh.h"
#include "Device.h"

class Mesh;
class Renderer {
public:
    Renderer();
    ~Renderer();
    void drawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3);
    void addMesh(Mesh *mesh);
    void render();
    void buffer2Screen();

private:
    I16 width_ = 0;
    I16 height_ = 0;

    Matrix camera = Matrix::IDENTITY;
    Mesh **meshs = nullptr;
    I32 meshCount = 0;
    Color renderBuffer[BUFFER_SIZE][BUFFER_SIZE];  // max screen 2048x2048
    Device &device_ = Device::getInstance();
};

#endif