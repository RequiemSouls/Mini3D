#include <thread>
#include <chrono>
#include <time.h>
#include "Renderer.h"
#include "Mesh.h"

#define FRAME_TIME 16
using namespace std;
int
main(int argc, char* args[]) {
    Renderer* r = new Renderer(1024, 768);
    Mesh* mesh = Mesh::genTriangle();
    Matrix m{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    while(true){
        clock_t ct = clock();
        // render
        mesh->draw(r, m);
        r->buffer2Screen();
        int dt = (clock() - ct) * 1000 / CLOCKS_PER_SEC;
        this_thread::sleep_for(chrono::milliseconds(FRAME_TIME - dt));
    }
    return 0;
}