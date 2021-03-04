#ifndef MINI3D_RENDERER_H_
#define MINI3D_RENDERER_H_

#include "camera.h"
#include "device.h"
#include "config.h"

MINI_NS_BEGIN

class Renderer {
public:
    // No copy.
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    // No move.
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;

    explicit Renderer(Device &device);
    ~Renderer();

    void DrawTriangle(Vertex &vt1, Vertex &vt2, Vertex &vt3,
                      const Matrix &m);
    void Clean();
    void Buffer2Screen() const;

    Camera& camera();

private:
    void Cut(Vertex& vt1, Vertex& vt2, Vertex& vt3);
    char CheckPoint(Vector &p);
    Vertex FindIntersectionVertex(char section, Vertex &vt1, Vertex &vt2);
    void Rasterize(Vertex vt1, Vertex vt2, Vertex vt3);
    void DrawLineDDA(Vector& p1, Vector& p2);
    void DrawLineMidPoint(Vector& p1, Vector& p2);
    void DrawLineBresenham(Vector& p1, Vector& p2);
    void DrawTriangleScanLine(Vertex* vt1, Vertex* vt2, Vertex* vt3);
    void DrawPixel(I32 x, I32 y, F32 z, const Color &c);
    void DrawHorizontalLine(Vertex &lv, Vertex &rv);
    Vertex GetInterp(const Vertex &v1, const Vertex &v2, I32 total);
    Vertex OnStep(const Vertex &start, const Vertex &k, I32 step);
    Matrix GetScreenMatrix();
private:
    I16 width_ = 0;
    I16 height_ = 0;
    Camera camera_;
    Color **render_buffer_ = nullptr;
    F32 **deep_buffer_ = nullptr;
    Device &device_;
};

MINI_NS_END

#endif  // MINI3D_RENDERER_H_
