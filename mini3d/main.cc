#include "display.h"
#include "renderer.h"

#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#endif

int main(int /*argc*/, char ** /*args*/) {
#ifndef _WIN32
    srand(time(0));
#else
    SYSTEMTIME st;
    GetSystemTime(&st);
    srand(st.wMilliseconds);
#endif

    mini3d::Device &device = mini3d::Device::GetInstance();
    mini3d::Renderer *r = new mini3d::Renderer(device);
    //mini3d::Display *root =
    //  new mini3d::Display("../asset/avg_captainamerica_0.obj");
    mini3d::Display *root = new mini3d::Display("../asset/cube.obj");
    mini3d::Vector pos {0, 0, 700, 1};
    mini3d::Vector scale {2, 2, 2, 0};
    mini3d::Vector rotate {0, 1, 0, 0};
    mini3d::F32 delta = 0;
    mini3d::Camera &camera = r->camera();

    device.SetLoopEvent([&rotate, &delta, &scale, &camera, &pos, &r, &root]() {
        // transfer test
        // pos.set_z(((int)pos.get_z() + 1) % 500 + 500);
        root->set_pos(pos);
        // camera lookat test
        // camera.set_lookat(pos);
        // rotate test
        delta += 0.03;
        root->set_rotate_v(rotate);
        root->set_rotate_d(delta);
        // scale test
        root->set_scale(scale);
        r->Render();
        root->Draw(r);
        r->Buffer2Screen();
    });

    mini3d::I32 ret = device.Loop();
    return ret;
}
