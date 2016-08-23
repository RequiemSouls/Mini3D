#include "renderer.h"
#include "assert.h"

#define MAX_MESH_COUNT 256

namespace mini3d {

Renderer::Renderer() {
    device_.GetMaxSize(width_, height_);
    meshs_ = (Mesh **)malloc(sizeof(Mesh *) * MAX_MESH_COUNT);
    render_buffer_ =  (Color **)malloc(sizeof(Color *) * width_);
    for (int i = 0; i < width_; ++i) {
        render_buffer_[i] = (Color *)malloc(sizeof(Color) * height_);
    }
}

Renderer::~Renderer() {
    for (int i = 0; i < MAX_MESH_COUNT; ++i)
    {
        free(meshs_[i]);
    }
    free(meshs_);
    for (int i = 0; i < width_; ++i)
    {
        free(render_buffer_[i]);
    }
    free(render_buffer_);    
}

void Renderer::AddMesh(Mesh *mesh) {
    if (mesh_count_ >= MAX_MESH_COUNT)
    {
        assert(0);
    }
    meshs_[mesh_count_] = mesh;
    mesh_count_++;
}

void Renderer::Render() {
    for (int i = 0; i < width_; ++i) {
        memset(render_buffer_[i], 0, sizeof(Color) * height_);
    }
    for (I32 im = 0; im < mesh_count_; ++im) {
        meshs_[im]->Draw(this, camera_);
    }
}

void Renderer::DrawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3) {
    // draw to buff
    // printf("{{%f,%f,%f}{%f,%f,%f}{%f,%f,%f}}\n",
    //        vt1->p.x, vt1->p.y, vt1->p.z,
    //        vt2->p.x, vt2->p.y, vt2->p.z,
    //        vt3->p.x, vt3->p.y, vt3->p.z);
    render_buffer_[I16(vt1->p.x * width_)][I16(vt1->p.y * height_)] = vt1->c;
    render_buffer_[I16(vt2->p.x * width_)][I16(vt2->p.y * height_)] = vt2->c;
    render_buffer_[I16(vt3->p.x)][I16(vt3->p.y)] = vt3->c;
}

void Renderer::Buffer2Screen() { device_.Buffer2Screen(render_buffer_); }

}  // namespace mini3d
