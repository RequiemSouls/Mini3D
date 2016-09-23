#include "renderer.h"
#include <assert.h>
#include <cmath>

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
        // DrawLineDDA(p1, p2);
        // DrawLineDDA(p2, p3);
        // DrawLineDDA(p3, p1);
        DrawLineMidPoint(p1, p2);
        DrawLineMidPoint(p2, p3);
        DrawLineMidPoint(p1, p3);
    }
}

void Renderer::DrawLineDDA(Vector& p1, Vector& p2) {
    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    F32 x1, y1, x2, y2;
    x1 = p1.get_x() * w + w;
    x2 = p2.get_x() * w + w;
    y1 = -p1.get_y() * h + h;
    y2 = -p2.get_y() * h + h;

    I32 total = 0;
    F32 dx, dy, absdx, absdy;
    F32 kx, ky, x, y;
    dx = x1 - x2;
    dy = y1 - y2;
    absdx = std::abs(dx);
    absdy = std::abs(dy);

    if (absdx >= absdy) {
        total = absdx;
    } else if (absdy > absdx) {
        total = absdy;
    }
    if (total == 0) {
        return;
    }

    x = x1;
    y = y1;
    kx = dx / total;
    ky = dy / total;
    Color rc = Color::WHITE;
    for (I32 i = 0; i < total; ++i) {
        x -= kx;
        y -= ky;
        DrawPixel(x, y, rc);
    }
}

void Renderer::DrawLineMidPoint(Vector& p1, Vector& p2) {
    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    F32 x1, y1, x2, y2;
    x1 = p1.get_x() * w + w;
    x2 = p2.get_x() * w + w;
    y1 = p1.get_y() * h + h;
    y2 = p2.get_y() * h + h;

    F32 dx, dy, absdx, absdy;
    dx = x1 - x2;
    dy = y1 - y2;
    absdx = std::abs(dx);
    absdy = std::abs(dy);
    if (absdx < 0.00001 && absdy < 0.00001) {
        return;
    }

    I32 x, y;
    x = x1 + 0.5;
    y = y1 + 0.5;
    I32 total = 0;
    I32 kx = dx < 0.00001 ? 0 : dx/absdx;
    I32 ky = dy < 0.00001 ? 0 : dy/absdy;
    F32 a, b, c;
    a = y2 - y1;
    b = x1 - x2;
    c = x2 * y1 - x1 * y2;
    if (absdx >= absdy) {
        total = absdx;
        for (I32 i = 0; i < total; ++i) {
            x -= kx;
            if ((a * x + b * (y - ky * 0.5) + c) > 0) {
                y -= ky;
            }
            DrawPixel(x, height_ - y, Color::WHITE);
        }
    } else if (absdy > absdx) {
        total = absdy;
        for (I32 i = 0; i < total; ++i) {
            y -= ky;
            if ((a * (x - kx * 0.5) + b * y + c) < 0) {
                x -= kx;
            }
            DrawPixel(x, height_ - y, Color::WHITE);
        }
    }
}

void Renderer::DrawPixel(I32 x, I32 y, Color c) {
    render_buffer_[x][y] = c;
}

void Renderer::Buffer2Screen() const { device_.Buffer2Screen(render_buffer_); }

Camera& Renderer::camera() { return camera_; }

}  // namespace mini3d