#ifndef _MINI3D_RENDERER_H_
#define _MINI3D_RENDERER_H_

#include "geometry.h"

class Renderer {
public:
    Renderer(int w, int h);
    void drawTriangle(Vertex* vt1, Vertex* vt2, Vertex* vt3);
    void buffer2Screen();

private:
    int w;
    int h;
    int renderBuffer[2048][2048]; // max screen 2048x2048
};

#endif