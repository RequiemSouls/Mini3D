#include "display.h"

namespace mini3d {

Display::Display() : mesh_(Mesh::GenTriangle()) {}

Display::Display(const char* mesh_file) : mesh_(Mesh::GenByFile(mesh_file)) {}

void Display::Draw(Renderer* render) { Draw(render, Matrix::IDENTITY); }

void Display::Draw(Renderer* render, const Matrix& m) {
    Matrix dm = Matrix::IDENTITY;
    dm.Transfer(pos_);
    dm = dm * m;

    mesh_.Draw(render, dm);
    for (Display* display : children_) {
        display->Draw(render, dm);
    }
}

void Display::AddChild(Display* display) { children_.push_back(display); }

void Display::set_pos(const Vector& pos) { pos_ = Vector(pos); }

const Vector& Display::pos() const { return pos_; }

}  // namespace mini3d
