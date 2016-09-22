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

    void DrawTriangle(const Vertex &vt1, const Vertex &vt2, const Vertex &vt3,
                      const Matrix &m);
    void Render();
    void Buffer2Screen() const;

    Camera& camera();

private:
    I16 width_ = 0;
    I16 height_ = 0;
    Camera camera_;
    Color **render_buffer_ = nullptr;
    Device &device_;
};

}  // namespace mini3d

#endif  // MINI3D_RENDERER_H_
