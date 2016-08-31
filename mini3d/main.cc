#include "display.h"
#include "renderer.h"

#include <stdlib.h>

int main(int /*argc*/, char ** /*args*/) {
    srand(time(0));

    mini3d::Device &device = mini3d::Device::GetInstance();
    mini3d::Renderer *r = new mini3d::Renderer(device);
    mini3d::Display *root =
        new mini3d::Display("../asset/avg_captainamerica_0.obj");
    mini3d::Vector pos {0, 0, 200, 1};

    device.SetLoopEvent([&pos, &r, &root]() {
        pos.IncZ();
        root->set_pos(pos);
        r->Render();
        root->Draw(r);
        r->Buffer2Screen();
    });

    mini3d::I32 ret = device.Loop();
    mini3d::Device::GetInstance().ExitDraw();
    return ret;
}
