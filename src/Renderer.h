#ifndef _MINI3D_RENDERER_H_
#define _MINI3D_RENDERER_H_

#include "DrawableUtils.h"
#include "Mesh.h"
#include "geometry.h"

class Mesh;
class Renderer {
   public:
    Renderer(I16 w, I16 h);
    ~Renderer();
    void drawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3);
    void addMesh(Mesh *mesh);
    void render();
    void buffer2Screen();

   private:
    I16 w = 0;
    I16 h = 0;

    Matrix camera = Matrix::IDENTITY;
    Mesh **meshs = nullptr;
    I32 meshCount = 0;
    Color renderBuffer[BUFFER_SIZE][BUFFER_SIZE];  // max screen 2048x2048
    DrawableUtils &drawable = DrawableUtils::getInstance();
};

#endif