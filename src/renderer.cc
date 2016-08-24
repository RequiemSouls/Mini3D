#include "renderer.h"
#include "assert.h"

#define MAX_MESH_COUNT 256

namespace mini3d {

Renderer::Renderer() {
    device_.GetMaxSize(width_, height_);
    meshs_ = (Mesh **)malloc(sizeof(Mesh *) * MAX_MESH_COUNT);
    render_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
    for (int i = 0; i < width_; ++i) {
        render_buffer_[i] = (Color *)calloc(height_, sizeof(Color));
    }
    Buffer2Screen();
}

Renderer::~Renderer() {
    for (int i = 0; i < MAX_MESH_COUNT; ++i) {
        delete meshs_[i];
    }
    free(meshs_);
    for (int i = 0; i < width_; ++i) {
        free(render_buffer_[i]);
    }
    free(render_buffer_);
}

void Renderer::AddMesh(Mesh *mesh) {
    if (mesh_count_ >= MAX_MESH_COUNT) {
        assert(0);
    }
    meshs_[mesh_count_] = mesh;
    mesh_count_++;
}

void Renderer::Render() {
    static I32 sizePerBuffer = sizeof(Color) * height_;
    for (int i = 0; i < width_; ++i) {
        memset(render_buffer_[i], 0, sizePerBuffer);
        for (int j = 0; j < height_; ++j)
        {
            if ((render_buffer_[i][j].r != 0) || (render_buffer_[i][j].g != 0) || (render_buffer_[i][j].b != 0))
            {
                assert(0);
            } 
        }

    }
    for (I32 im = 0; im < mesh_count_; ++im) {
        meshs_[im]->Draw(this, camera_);
    }
}

void Renderer::DrawTriangle(Vertex *vt1, Vertex *vt2, Vertex *vt3) {
    render_buffer_[I16(vt1->p.x * width_)][I16(vt1->p.y * height_)] = vt1->c;
    render_buffer_[I16(vt2->p.x * width_)][I16(vt2->p.y * height_)] = vt2->c;
    render_buffer_[I16(vt3->p.x)][I16(vt3->p.y)] = vt3->c;
}

void Renderer::Buffer2Screen() { device_.Buffer2Screen(render_buffer_); }

}  // namespace mini3d
