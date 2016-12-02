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
    mini3d::Display *root =
     new mini3d::Display("../../asset/avg_captainamerica_0.obj");
    // mini3d::Display *root = new mini3d::Display("../../asset/line.obj");
    mini3d::Vector pos {0, -400, 800, 1};
    mini3d::Vector scale {4, 4, 4, 0};
    mini3d::Vector rotate {0, 1, 0, 0};
    mini3d::F32 delta = 0;
    mini3d::F32 deltaOffset = 0;
    bool stop = false;
    mini3d::Camera &camera = r->camera();

    device.SetKeyDownEvent([&stop, &deltaOffset, &rotate](mini3d::I32 type){
        switch (type) {
        case SDLK_q:
            stop = true;
            break;
        case SDLK_d:
            stop = false;
            rotate.set_x(0);
            rotate.set_y(1);
            rotate.set_z(0);
            deltaOffset = -0.03;
            break;
        case SDLK_a:
            stop = false;
            rotate.set_x(0);
            rotate.set_y(1);
            rotate.set_z(0);
            deltaOffset = 0.03;
            break;
        case SDLK_s:
            rotate.set_x(1);
            rotate.set_y(0);
            rotate.set_z(0);
            deltaOffset = -0.03;
            stop = false;
            break;
        case SDLK_w:
            rotate.set_x(1);
            rotate.set_y(0);
            rotate.set_z(0);
            deltaOffset = 0.03;

            stop = false;
            break;
        }
    });
    device.SetLoopEvent([&stop, &deltaOffset, &rotate, &delta, &scale, &camera, &pos, &r, &root]() {
        if (stop) {
            return;
        }
        // transfer test
        // pos.set_z(((int)pos.get_z() + 1) % 500 + 500);
        root->set_pos(pos);
        // camera lookat test
        // camera.set_lookat(pos);
        // rotate test
        delta += deltaOffset;
        root->set_rotate_v(rotate);
        root->set_rotate_d(delta);
        // scale test
        root->set_scale(scale);
        r->Clean();
        root->Draw(r);
        r->Buffer2Screen();
    });

    mini3d::I32 ret = device.Loop();
    return ret;
}
