#include "renderer.h"

int main(int /*argc*/, char ** /*args*/) {
    srand(time(0));

    mini3d::Device &device = mini3d::Device::GetInstance();
    mini3d::Renderer *r = new mini3d::Renderer();

    for (int i = 0; i < 250; ++i)
    {
        mini3d::Mesh *mesh = mini3d::Mesh::GenTriangle();
        r->AddMesh(mesh);
    }
    device.SetLoopEvent([&]() {
        // render
        r->Render();
        r->Buffer2Screen();
    });

    mini3d::I32 ret = device.Loop();
    mini3d::Device::GetInstance().ExitDraw();
    return ret;
}
