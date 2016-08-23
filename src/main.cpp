#include "Renderer.h"

int main(int argc, char *args[]) {
    srand(time(0));
    Device &device = Device::getInstance();
    Renderer *r = new Renderer();

    for (int i = 0; i < 250; ++i)
    {
        Mesh *mesh = Mesh::genTriangle();
        r->addMesh(mesh);
    }
    device.setLoopEvent([&]() {
        // render
        r->render();
        r->buffer2Screen();
    });

    I32 ret = device.loop();
    Device::getInstance().exitDraw();
    return ret;
}