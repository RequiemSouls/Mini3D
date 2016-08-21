#include <thread>
#include <chrono>
#include <time.h>
#include "Renderer.h"
#include "Mesh.h"
#include "DrawableUtils.h"

#define FRAME_TIME 200
using namespace std;
int main(int argc, char* args[]) {
    int w, h;
    DrawableUtils::getInstance().getMaxSize(w, h);
    Renderer* r = new Renderer(w, h);
    Mesh* mesh = Mesh::genTriangle();
    r->addMesh(mesh);
    while(true) {
        clock_t ct = clock();
        // render
        DrawableUtils::getInstance().clearScreen();
        r->render();
        r->buffer2Screen();
        int dt = (clock() - ct) * 1000 / CLOCKS_PER_SEC;
        this_thread::sleep_for(chrono::milliseconds(FRAME_TIME - dt));
    }
    DrawableUtils::getInstance().exitDraw();
    return 0;
}