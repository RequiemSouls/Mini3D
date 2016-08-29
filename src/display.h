#ifndef _MINI3D_DISPLAY_H_
#define _MINI3D_DISPLAY_H_

#include <vector>
#include "geometry.h"
#include "mesh.h"
#include "renderer.h"

namespace mini3d {

class Display {
public:
    // No copy.
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    // Moveable.
    Display(Display&&) = default;
    Display& operator=(Display&&) = default;

    Display();
    explicit Display(const char* mesh_file);
    virtual ~Display() {}

    void Draw(Renderer* render, const Matrix& m);
    void Draw(Renderer* render);

    void AddChild(Display* display);

    void set_pos(const Vector& pos);
    const Vector& pos() const;

private:
    Mesh mesh_;
    std::vector<Display*> children_;
    Vector pos_;
};

}  // namespace mini3d

#endif
