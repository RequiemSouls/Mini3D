#include "display.h"

MINI_NS_BEGIN

Display::Display() :
pos_(Vector{ 0, 0, 0, 0 }),
mesh_(Mesh::GenTriangle()),
rotate_v_(Vector()),
rotate_d_(0.0f),
scale_(Vector{ 1, 1, 1, 0 })
{}

Display::Display(const char* mesh_file) :
pos_(Vector{ 0, 0, 0, 0 }),
mesh_(Mesh::GenByFile(mesh_file)),
rotate_v_(Vector{ 1, 1, 1, 0 }),
rotate_d_(0.0f),
scale_(Vector{ 1, 1, 1, 0 })
{}

void Display::Draw(Renderer* render) { Draw(render, Matrix::IDENTITY()); }

void Display::Draw(Renderer* render, const Matrix& m) {
    Matrix transfer = Matrix::IDENTITY();
    transfer.Transfer(pos_);

    Matrix rotate = Matrix::IDENTITY();
    rotate.Rotate(rotate_v_, rotate_d_);

    Matrix scale = Matrix::IDENTITY();
    scale.Scale(scale_);

    Matrix finalM = transfer * rotate * scale * m;

    mesh_.Draw(render, finalM);
    for (Display* display : children_) {
        display->Draw(render, transfer * rotate * scale * m);
    }
}

void Display::AddChild(Display* display) { children_.push_back(display); }

void Display::set_pos(const Vector& pos) { pos_ = Vector(pos); }

const Vector& Display::pos() const { return pos_; }

void Display::set_rotate_v(const Vector& vec) { rotate_v_ =  Vector(vec); }

const Vector& Display::rotate_v() const { return rotate_v_; }

void Display::set_rotate_d(const F32& delta) { rotate_d_ = delta; }

const F32& Display::rotate_d() const { return rotate_d_; }

void Display::set_scale(const Vector& scale) { scale_ = Vector(scale); }

const Vector& Display::scale() const { return scale_; }

MINI_NS_END
