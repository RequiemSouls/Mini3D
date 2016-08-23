#include "Renderer.h"
#include "assert.h"

#define MAX_MESH_COUNT 256

Renderer::Renderer() {
    device_.getMaxSize(width_, height_);
    meshs = (Mesh **)malloc(sizeof(Mesh *) * MAX_MESH_COUNT);
}

Renderer::~Renderer() { free(meshs); }

void Renderer::addMesh(Mesh *mesh) {
    if (meshCount >= MAX_MESH_COUNT)
    {
        assert(0);
    }
    meshs[meshCount] = mesh;
    meshCount++;
}

void Renderer::render() {
    memset(renderBuffer, 0, sizeof(renderBuffer));
    for (I32 im = 0; im < meshCount; ++im) {
        meshs[im]->draw(this, camera);
    }
}

void Renderer::drawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3) {
    // draw to buff
    // printf("{{%f,%f,%f}{%f,%f,%f}{%f,%f,%f}}\n",
    //        vt1->p.x, vt1->p.y, vt1->p.z,
    //        vt2->p.x, vt2->p.y, vt2->p.z,
    //        vt3->p.x, vt3->p.y, vt3->p.z);
    renderBuffer[I16(vt1->p.x * width_)][I16(vt1->p.y * height_)] = vt1->c;
    renderBuffer[I16(vt2->p.x * width_)][I16(vt2->p.y * height_)] = vt2->c;
    renderBuffer[I16(vt3->p.x)][I16(vt3->p.y)] = vt3->c;
}

void Renderer::buffer2Screen() { device_.buffer2Screen(renderBuffer); }