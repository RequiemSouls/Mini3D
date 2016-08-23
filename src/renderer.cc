#include "renderer.h"
#include "assert.h"

#define MAX_MESH_COUNT 256

Renderer::Renderer() {
    device_.GetMaxSize(width_, height_);
    meshs_ = (Mesh **)malloc(sizeof(Mesh *) * MAX_MESH_COUNT);
}

Renderer::~Renderer() { free(meshs_); }

void Renderer::AddMesh(Mesh *mesh) {
    if (meshCount_ >= MAX_MESH_COUNT)
    {
        assert(0);
    }
    meshs_[meshCount_] = mesh;
    meshCount_++;
}

void Renderer::Render() {
    memset(renderBuffer_, 0, sizeof(renderBuffer_));
    for (I32 im = 0; im < meshCount_; ++im) {
        meshs_[im]->Draw(this, camera_);
    }
}

void Renderer::DrawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3) {
    // draw to buff
    // printf("{{%f,%f,%f}{%f,%f,%f}{%f,%f,%f}}\n",
    //        vt1->p.x, vt1->p.y, vt1->p.z,
    //        vt2->p.x, vt2->p.y, vt2->p.z,
    //        vt3->p.x, vt3->p.y, vt3->p.z);
    renderBuffer_[I16(vt1->p.x * width_)][I16(vt1->p.y * height_)] = vt1->c;
    renderBuffer_[I16(vt2->p.x * width_)][I16(vt2->p.y * height_)] = vt2->c;
    renderBuffer_[I16(vt3->p.x)][I16(vt3->p.y)] = vt3->c;
}

void Renderer::Buffer2Screen() { device_.Buffer2Screen(renderBuffer_); }