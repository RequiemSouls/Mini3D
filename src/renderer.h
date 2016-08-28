#ifndef MINI3D_RENDERER_H_
#define MINI3D_RENDERER_H_

#include "device.h"
#include "camera.h"

namespace mini3d {

class Renderer {
public:
    Renderer();
    ~Renderer();
    void DrawTriangle(Vertex &vt1, Vertex &vt2, Vertex &vt3, Matrix &m);
    void Render();
    void Buffer2Screen();

private:
    I16 width_ = 0;
    I16 height_ = 0;
    Camera* camera_;
    Color **render_buffer_;
    Device &device_ = Device::GetInstance();
};

}  // namespace mini3d

#endif  // MINI3D_RENDERER_H_
