#include "display.h"

namespace mini3d {

Display::Display() :
    mesh_(Mesh::GenTriangle()),
    rotate_v_(Vector()),
    rotate_d_(0.0f)
{}

Display::Display(const char* mesh_file) : mesh_(Mesh::GenByFile(mesh_file)) {}

void Display::Draw(Renderer* render) { Draw(render, Matrix::IDENTITY()); }

void Display::Draw(Renderer* render, const Matrix& m) {
    Matrix transfer = Matrix::IDENTITY();
    transfer.Transfer(pos_);

    Matrix rotate = Matrix::IDENTITY();
    rotate.Rotate(rotate_v_, rotate_d_);

    mesh_.Draw(render, transfer * rotate * m);
    for (Display* display : children_) {
        display->Draw(render, transfer * rotate * m);
    }
}

void Display::AddChild(Display* display) { children_.push_back(display); }

void Display::set_pos(const Vector& pos) { pos_ = Vector(pos); }

const Vector& Display::pos() const { return pos_; }

void Display::set_rotate_v(const Vector& vec) { rotate_v_ =  Vector(vec); }

const Vector& Display::rotate_v() const { return rotate_v_; }

void Display::set_rotate_d(const F32& delta) { rotate_d_ = delta; }

const F32& Display::rotate_d() const { return rotate_d_; }
}  // namespace mini3d
