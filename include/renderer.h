#ifndef MINI3D_RENDERER_H_
#define MINI3D_RENDERER_H_

#include "camera.h"
#include "device.h"

namespace mini3d {

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
    void Render();
    void Buffer2Screen() const;

    Camera& camera();

private:
    void Rasterize(Vertex& vt1, Vertex& vt2, Vertex& vt3);
    void DrawLineDDA(Vector& p1, Vector& p2);
    void DrawLineMidPoint(Vector& p1, Vector& p2);
	void DrawLineBresenham(Vector& p1, Vector& p2);
    void DrawTriangleScanLine(Vertex* vt1, Vertex* vt2, Vertex* vt3);
    I32 GetInterp(I32 lv, I32 rv, I32 offset, I32 step);
    void DrawPixel(I32 x, I32 y, Color c);
    void DrawHorizontalLine(I32 y, I32 lx, I32 rx, Color lc, Color rc);
private:
    I32 first_draw_ = 0;
    I16 width_ = 0;
    I16 height_ = 0;
    Camera camera_;
    Color **render_buffer_ = nullptr;
    Device &device_;
};

}  // namespace mini3d

#endif  // MINI3D_RENDERER_H_
