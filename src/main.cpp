#ifndef _WIN32

#include <time.h>
#include <chrono>
#include <thread>
#include "DrawableUtils.h"
#include "Mesh.h"
#include "Renderer.h"

#define FRAME_TIME 200
using namespace std;

int main(int argc, char *args[]) {
    I16 w, h;

    srand(time(0));
    DrawableUtils::getInstance().getMaxSize(w, h);
    Renderer *r = new Renderer(w, h);
    Mesh *mesh = Mesh::genTriangle();
    r->addMesh(mesh);
    while (true) {
        clock_t ct = clock();
        // render
        DrawableUtils::getInstance().clearScreen();
        r->render();
        r->buffer2Screen();
        I32 dt = (clock() - ct) * 1000 / CLOCKS_PER_SEC;
        this_thread::sleep_for(chrono::milliseconds(FRAME_TIME - dt));
    }
    DrawableUtils::getInstance().exitDraw();
    return 0;
}

#else

#include "Device.h"
#include "DrawableUtils.h"
#include "Mesh.h"
#include "Renderer.h"

using namespace std;
int main(int argc, char *args[]) {
    srand(time(0));
    Renderer *r = new Renderer(960, 640);
    Mesh *mesh = Mesh::genTriangle();
    r->addMesh(mesh);

    Device device;

    device.setLoopEvent([&]() {
        // render
        r->render();

        device.drawPixel(10, 10, Color(255, 0, 255));
        device.drawLine(Vec2(20, 200), Vec2(500, 500), Color(255, 0, 0));

        r->buffer2Screen();
    });

    I32 ret = device.loop();
    return ret;
}

#endif