#include <stdlib.h>
#include <stdio.h>
#include "Renderer.h"

Renderer::Renderer(int w, int h) {
    this->w = w;
    this->h = h;
    meshs = (Mesh**)malloc(sizeof(Mesh*) * 256);
}

Renderer::~Renderer() {
    free(meshs);
}

void Renderer::addMesh(Mesh* mesh) {
    meshs[meshCount] = mesh;
    meshCount++;
}

void Renderer::render() {
    for (int im = 0; im < meshCount; ++im) {
        meshs[im]->draw(this, camera);
    }
}

void Renderer::drawTriangle(Vertex* vt1, Vertex* vt2, Vertex* vt3) {
    //draw to buff
    printf("{{%f,%f,%f}{%f,%f,%f}{%f,%f,%f}}\n",
           vt1->p.x, vt1->p.y, vt1->p.z,
           vt2->p.x, vt2->p.y, vt2->p.z,
           vt3->p.x, vt3->p.y, vt3->p.z);
}

void Renderer::buffer2Screen() {
    for (int iw = 0; iw < w; ++iw) {
        for (int ih = 0; ih < h; ++ih) {
            //drawPoint(iw, ih, renderBuffer[iw][ih]);
        }
    }
}