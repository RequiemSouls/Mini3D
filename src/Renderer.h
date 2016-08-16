#ifndef _MINI3D_RENDERER_H_
#define _MINI3D_RENDERER_H_

#include "Mesh.h"
#include "geometry.h"

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
    int w;
    int h;
    Matrix camera = Matrix::identity();
    Mesh** meshs;
    int meshCount = 0;
    int renderBuffer[2048][2048]; // max screen 2048x2048
};

#endif