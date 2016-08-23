#include "renderer.h"

int main(int argc, char *args[]) {
    srand(time(0));
    Device &device = Device::GetInstance();
    Renderer *r = new Renderer();

    for (int i = 0; i < 250; ++i) {
        Mesh *mesh = Mesh::GenTriangle();
        r->AddMesh(mesh);
    }
    device.SetLoopEvent([&]() {
        // render
        r->Render();
        r->Buffer2Screen();
    });

    I32 ret = device.Loop();
    Device::GetInstance().ExitDraw();
    return ret;
}
