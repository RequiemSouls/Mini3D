#ifndef _MINI3D_RENDERER_H_
#define _MINI3D_RENDERER_H_

#include "Mesh.h"
#include "geometry.h"
#include "DrawableUtils.h"

class Mesh;
class Renderer {
public:
    Renderer(int w, int h);
    ~Renderer();
    void drawTriangle(Vertex* vt1, Vertex* vt2, Vertex* vt3);
    void addMesh(Mesh* mesh);
    void render();
    void buffer2Screen();

private:
    int w = 0;
    int h = 0;

    Matrix camera = Matrix::IDENTITY;
    Mesh** meshs = nullptr;
    int meshCount = 0;
    Color renderBuffer[2048][2048]; // max screen 2048x2048
    DrawableUtils& drawable = DrawableUtils::getInstance();
};

#endif