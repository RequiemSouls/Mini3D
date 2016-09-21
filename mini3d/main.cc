#include "display.h"
#include "renderer.h"

#include <stdlib.h>

int main(int /*argc*/, char ** /*args*/) {
    srand(time(0));

    mini3d::Device &device = mini3d::Device::GetInstance();
    mini3d::Renderer *r = new mini3d::Renderer(device);
    mini3d::Display *root =
        new mini3d::Display("../asset/avg_captainamerica_0.obj");
    mini3d::Vector pos {0, 0, 700, 1};
    root->set_pos(pos);
    mini3d::Vector rotate {1, 1, 1, 1};
    mini3d::F32 delta = 0;
    root->set_rotate_v(rotate);
    root->set_rotate_d(0);

    device.SetLoopEvent([&rotate, &delta, &pos, &r, &root]() {
        // transfer test
        pos.set_x(((int)pos.get_x() + 1) % 500);
        root->set_pos(pos);
        // rotate test
        delta += 0.03;
        root->set_rotate_d(delta);
        r->Render();
        root->Draw(r);
        r->Buffer2Screen();
    });

    mini3d::I32 ret = device.Loop();
    mini3d::Device::GetInstance().ExitDraw();
    return ret;
}
