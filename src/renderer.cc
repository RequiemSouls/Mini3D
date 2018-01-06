#include "renderer.h"
#include <assert.h>
#include <cmath>

#define MAX_MESH_COUNT 256
#define px(n) n.get_x()
#define py(n) n.get_y()
#define pz(n) n.get_z()
#define vx(n) n->p.get_x()
#define vy(n) n->p.get_y()
#define vz(n) n->p.get_z()

MINI_NS_BEGIN

Renderer::Renderer(Device &device) : device_(device) {
    device_.GetMaxSize(&width_, &height_);
    camera_.set_ratio(height_ * 1.0f / width_);
    // printf("%d, %d, %f", width_, height_, height_ * 1.0f / width_);
    render_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
    for (int i = 0; i < width_; ++i) {
        render_buffer_[i] = (Color *)calloc(height_, sizeof(Color));
    }

    deep_buffer_ = (F32 **)malloc(sizeof(F32 *) * width_);
    for (int i = 0; i < width_; ++i) {
        deep_buffer_[i] = (F32 *)calloc(height_, sizeof(F32));
    }
}

void Renderer::Clean() {
    static I32 deepRowSize = sizeof(F32) * height_;
    for (int i = 0; i < width_; ++i) {
        memset(deep_buffer_[i], 0, deepRowSize);
    }
    // printf("%f \n", deep_buffer_[100][100]);
    static I32 renderRowSize = sizeof(Color) * height_;
    for (int i = 0; i < width_; ++i) {
        memset(render_buffer_[i], 0, renderRowSize);
    }
}

Renderer::~Renderer() {
    for (int i = 0; i < width_; ++i) {
        free(render_buffer_[i]);
    }
    free(render_buffer_);
}

Matrix Renderer::GetScreenMatrix() {
    Matrix m;
    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    m.set_value(0, 0, w);
    m.set_value(1, 1, h);
    m.set_value(0, 3, w);
    m.set_value(1, 3, h);
    m.set_value(2, 2, 1);
    m.set_value(3, 3, 1);
    return m;
}

void Renderer::DrawTriangle(Vertex &vt1, Vertex &vt2,
                            Vertex &vt3, const Matrix &m) {
    const Matrix mp = camera_.GetMatrix();
    vt1.p = mp * m * vt1.p;
    vt2.p = mp * m * vt2.p;
    vt3.p = mp * m * vt3.p;
    vt1.p.Homogenize();
    vt2.p.Homogenize();
    vt3.p.Homogenize();
    Rasterize(vt1, vt2, vt3);
}

void Renderer::Rasterize(Vertex& vt1, Vertex& vt2, Vertex& vt3) {
    // cull
    if (vt1.p.get_x() > 1 or vt1.p.get_x() < -1
        or vt1.p.get_y() > 1 or vt1.p.get_y() < -1
        or vt1.p.get_z() > 1 or vt1.p.get_z() < -1) {
        return;
    }
    if (vt2.p.get_x() > 1 or vt2.p.get_x() < -1
        or vt2.p.get_y() > 1 or vt2.p.get_y() < -1
        or vt2.p.get_z() > 1 or vt2.p.get_z() < -1) {
        return;
    }
    if (vt3.p.get_x() > 1 or vt3.p.get_x() < -1
        or vt3.p.get_y() > 1 or vt3.p.get_y() < -1
        or vt3.p.get_z() > 1 or vt3.p.get_z() < -1) {
        return;
    }
    const Matrix sm = GetScreenMatrix();
    vt1.p = sm * vt1.p;
    vt2.p = sm * vt2.p;
    vt3.p = sm * vt3.p;
    vt1.p.set_x(floor(vt1.p.get_x() + 0.5));
    vt1.p.set_y(floor(vt1.p.get_y() + 0.5));
    vt2.p.set_x(floor(vt2.p.get_x() + 0.5));
    vt2.p.set_y(floor(vt2.p.get_y() + 0.5));
    vt3.p.set_x(floor(vt3.p.get_x() + 0.5));
    vt3.p.set_y(floor(vt3.p.get_y() + 0.5));
    DrawTriangleScanLine(&vt1, &vt2, &vt3);
    //DrawLineDDA(vt1, vt2);
    //DrawLineDDA(vt2, vt3);
    //DrawLineDDA(vt3, vt1);
    //DrawLineMidVToint(vt1, vt2);
    //DrawLineMidVToint(vt2, vt3);
    //DrawLineMidVToint(vt1, vt3);
    // DrawLineBresenham(vt1.p, vt2.p);
    // DrawLineBresenham(vt2.p, vt3.p);
    // DrawLineBresenham(vt1.p, vt3.p);
}

Vertex Renderer::GetInterp(const Vertex &v1, const Vertex &v2, I32 total) {
    Vertex result;
    if (total == 0) {
        return result;
    }
    result.p.set_x((v1.p.get_x() - v2.p.get_x()) / total);
    result.p.set_y((v1.p.get_y() - v2.p.get_y()) / total);
    result.p.set_z((v1.p.get_z() - v2.p.get_z()) / total);

    result.c.r = (v1.c.r - v2.c.r) / total;
    result.c.g = (v1.c.g - v2.c.g) / total;
    result.c.b = (v1.c.b - v2.c.b) / total;
    return result;
}

Vertex Renderer::OnStep(const Vertex &start, const Vertex &k, I32 step) {
    Vertex result;
    result.p.set_x(I32(start.p.get_x() + k.p.get_x() * step + 0.5));
    result.p.set_y(I32(start.p.get_y() + k.p.get_y() * step + 0.5));
    result.p.set_z(start.p.get_z() + k.p.get_z() * step);

    result.c.r = start.c.r + k.c.r * step;
    result.c.g = start.c.g + k.c.g * step;
    result.c.b = start.c.b + k.c.b * step;
    return result;
}

void Renderer::DrawTriangleScanLine(Vertex* vt1, Vertex* vt2, Vertex* vt3) {
    Vertex* temp = nullptr;
    // sort
    if (vy(vt1) > vy(vt2)) { temp = vt1; vt1 = vt2; vt2 = temp; };
    if (vy(vt2) > vy(vt3)) { temp = vt2; vt2 = vt3; vt3 = temp; };
    if (vy(vt1) > vy(vt2)) { temp = vt1; vt1 = vt2; vt2 = temp; };
    if (vy(vt1) == vy(vt2) && vy(vt1) == vy(vt3)) return;
    if (vx(vt1) == vx(vt2) && vx(vt1) == vx(vt3)) return;

    I32 offsetTM = vy(vt3) - vy(vt2);
    I32 offsetMB = vy(vt2) - vy(vt1);
    const Vertex &tm = GetInterp(*vt3, *vt2, vt3->p.get_y() - vt2->p.get_y());
    const Vertex &tb = GetInterp(*vt3, *vt1, vt3->p.get_y() - vt1->p.get_y());
    const Vertex &mb = GetInterp(*vt2, *vt1, vt2->p.get_y() - vt1->p.get_y());
    for (I32 step = 1; step <= offsetMB; step++) {
        const Vertex &lv = OnStep(*vt1, mb, step);
        const Vertex &rv = OnStep(*vt1, tb, step);
        DrawHorizontalLine(lv, rv);
    }

    for (I32 step = 0; step <= offsetTM; step++) {
        const Vertex &lv = OnStep(*vt3, tm, -step);
        const Vertex &rv = OnStep(*vt3, tb, -step);
        DrawHorizontalLine(lv, rv);
    }
}

void Renderer::DrawHorizontalLine(const Vertex &lv, const Vertex &rv) {
    I32 gap = std::abs(lv.p.get_x() - rv.p.get_x());
    const Vertex &k = GetInterp(lv, rv, gap);
    for (int i = 0; i <= gap; ++i)
    {
        const Vertex &point = OnStep(rv, k, i);
        DrawPixel(point.p.get_x(), point.p.get_y(), point.p.get_z(), point.c);
    }
}

void Renderer::DrawLineBresenham(Vector& p1, Vector& p2) {
    F32 dx, dy, dz, absdx, absdy, absdz;
    dx = px(p1) - px(p2);
    dy = py(p1) - py(p2);
    dz = pz(p1) - pz(p2);
    absdx = std::abs(dx);
    absdy = std::abs(dy);
    absdz = std::abs(dz);
    if (absdx < 0.00001 && absdy < 0.00001) {
        return;
    }
    I32 x = px(p1), y = py(p1);
    F32 z = pz(p1);
    F32 d = 0;
    I32 stepx = absdx == 0 ? 0 : dx / absdx;
    I32 stepy = absdy == 0 ? 0 : dy / absdy;
    if (absdx >= absdy) {
        I32 total = absdx;
        F32 stepz = dz / total;
        F32 k = std::abs(dy / dx);
        for (I32 i = 0; i < total; ++i) {
            x -= stepx;
            d += k;
            if (d > 1) {
                y -= stepy;
                d -= 1;
            }
            z -= stepz;
            DrawPixel(x, y, z, Color::WHITE);
        }
    } else if (absdx < absdy) {
        I32 total = absdy;
        F32 stepz = dz / total;
        F32 k = std::abs(dx / dy);
        for (I32 i = 0; i < total; ++i) {
            y -= stepy;
            d += k;
            if (d > 1) {
                x -= stepx;
                d -= 1;
            }
            z -= stepz;
            DrawPixel(x, y, z, Color::WHITE);
        }
    }
}

void Renderer::DrawPixel(I32 x, I32 y, F32 z, const Color &c) {
    if (deep_buffer_[x][y] > z - 1) {
        deep_buffer_[x][y] = z - 1;
        render_buffer_[x][y] = c;
    }
}

void Renderer::Buffer2Screen() const { device_.Buffer2Screen(render_buffer_); }

Camera& Renderer::camera() { return camera_; }

MINI_NS_END
