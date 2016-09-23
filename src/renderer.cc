#include "renderer.h"
#include <assert.h>

#define MAX_MESH_COUNT 256

namespace mini3d {

Renderer::Renderer(Device &device) : device_(device) {
    device_.GetMaxSize(&width_, &height_);
    camera_.set_ratio(height_ * 1.0f / width_);
    printf("%d, %d, %f", width_, height_, height_ * 1.0f / width_);
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
    Matrix mp = camera_.GetMatrix();
    Vector p1 = mp * m * vt1.p;
    Vector p2 = mp * m * vt2.p;
    Vector p3 = mp * m * vt3.p;
    p1.Homogenize();
    p2.Homogenize();
    p3.Homogenize();
    Rasterize(p1, p2, p3);
}

void Renderer::Rasterize(Vector& p1, Vector& p2, Vector& p3) {
    if (p1.get_x() > -1 && p1.get_x() < 1 && p1.get_y() > -1 && p1.get_y() < 1 &&
    p2.get_x() > -1 && p2.get_x() < 1 && p2.get_y() > -1 && p2.get_y() < 1 &&
        p3.get_x() > -1 && p3.get_x() < 1 && p3.get_y() > -1 && p3.get_y() < 1 )
    {
        DrawLine(p1, p2);
        DrawLine(p2, p3);
        DrawLine(p3, p1);
    }
}

void Renderer::DrawLine(Vector& p1, Vector& p2) {
    Color rc = Color::WHITE;
    F32 w = width_ / 2;
    F32 h = height_ / 2;
    I32 x1, y1, x2, y2;
    x1 = p1.get_x() * w + w;
    y1 = p1.get_y() * h + h;
    x2 = p2.get_x() * w + w;
    y2 = p2.get_y() * h + h;
    I32 dx, dy, absdx, absdy;
    F32 kx, ky;
    F32 x, y;
    dx = x1 - x2;
    dy = y1 - y2;
    absdx = abs(dx);
    absdy = abs(dy);
    x = x1;
    y = y1;
    if (absdx > absdy) {
        kx = dx / absdx;
        ky = dy * 1.0f / absdx;
        for (I32 i = 0; i < absdx; ++i) {
            x -= kx;
            y -= ky;
            DrawPixel(x, y, rc);
        }
    } else if (abs(dy) > abs(dx)) {
        kx = dx * 1.0f / abs(dy);
        ky = dy / abs(dy);
        for (I32 i = 0; i < abs(dy); ++i) {
            x -= kx;
            y -= ky;
            DrawPixel(x, y, rc);
        }
    } else {
        // do nothing
    }
}

void Renderer::DrawPixel(int x, int y, Color c) {
    render_buffer_[x][y] = c;
}

void Renderer::Buffer2Screen() const { device_.Buffer2Screen(render_buffer_); }

Camera& Renderer::camera() { return camera_; }

}  // namespace mini3d