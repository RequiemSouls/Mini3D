#include "renderer.h"
#include <assert.h>

#define MAX_MESH_COUNT 256

namespace mini3d {

Renderer::Renderer(Device &device) : device_(device) {
    device_.GetMaxSize(&width_, &height_);
    render_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
    for (int i = 0; i < width_; ++i) {
        render_buffer_[i] = (Color *)calloc(height_, sizeof(Color));
    }
    Buffer2Screen();
}

Renderer::~Renderer() {
    for (int i = 0; i < width_; ++i) {
        free(render_buffer_[i]);
    }
    free(render_buffer_);
}

void Renderer::Render() {
    static I32 sizePerBuffer = sizeof(Color) * height_;
    for (int i = 0; i < width_; ++i) {
        memset(render_buffer_[i], 0, sizePerBuffer);
    }
}

void Renderer::DrawTriangle(const Vertex &vt1, const Vertex &vt2,
                            const Vertex &vt3, const Matrix &m) {
    F32 w = width_ / 2;
    F32 h = height_ / 2;
    Matrix mp = camera_.GetMatrix();
    // printf("%f %f %f\n", vt1.p.x, vt1.p.y, vt1.p.z);
    Vector p1 = mp * m * vt1.p;
    Vector p2 = mp * m * vt2.p;
    Vector p3 = mp * m * vt3.p;
    p1.Homogenize();
    p2.Homogenize();
    p3.Homogenize();
    // printf("%f %f %f\n", p1.x, p1.y, p1.z);
    if (p1.get_z() <= 1.0 && p1.get_z() >= -1.0 && p1.get_x() >= -1.0 && p1.get_x() <= 1.0 &&
        p1.get_y() <= 1.0 && p1.get_y() >= -1.0) {
        render_buffer_[(I32)(p1.get_x() * w + w)][(I32)(-p1.get_y() * h + h)] = vt1.c;
    }
    if (p2.get_z() <= 1.0 && p2.get_z() >= -1.0 && p2.get_x() >= -1.0 && p2.get_x() <= 1.0 &&
        p2.get_y() <= 1.0 && p2.get_y() >= -1.0) {
        render_buffer_[(I32)(p2.get_x() * w + w)][(I32)(-p2.get_y() * h + h)] = vt2.c;
    }
    if (p3.get_z() <= 1.0 && p3.get_z() >= -1.0 && p3.get_x() >= -1.0 && p3.get_x() <= 1.0 &&
        p3.get_y() <= 1.0 && p3.get_y() >= -1.0) {
        render_buffer_[(I32)(p3.get_x() * w + w)][(I32)(-p3.get_y() * h + h)] = vt3.c;
    }
}

void Renderer::Buffer2Screen() const { device_.Buffer2Screen(render_buffer_); }

}  // namespace mini3d
