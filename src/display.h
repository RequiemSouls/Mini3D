#ifndef _MINI3D_DISPLAY_H_
#define _MINI3D_DISPLAY_H_

#include <vector>
#include "renderer.h"
#include "mesh.h"
#include "geometry.h"

namespace mini3d {

class Display {
public:
    Display();
    Display(const char* mesh_file);
    ~Display();

    void Draw(Renderer* render, Matrix& m);
    void Draw(Renderer* render);

    void AddChild(Display* display);
    void set_pos(Vector &pos);
    Vector &pos();
private:
    Mesh* mesh_;
    std::vector<Display*> children_;
    Vector pos_;
};

}

#endif