#include "display.h"

namespace mini3d {

Display::Display() {
    mesh_ = Mesh::GenTriangle();
}

Display::Display(const char* file_name) {
    mesh_ = Mesh::GenByFile(file_name);
}

Display::~Display() {
    delete mesh_;
}

void Display::Draw(Renderer* r) {
    Draw(r, (Matrix&)Matrix::IDENTITY);
}

void Display::Draw(Renderer* r, Matrix& m) {
    Matrix dm = Matrix::IDENTITY;
    dm.Transfer(pos_);
    dm = dm * m;

    mesh_->Draw(r, dm);
    for(Display* display : children_) {
        display->Draw(r, dm);
    }
}

void Display::AddChild(Display* display) {
    children_.push_back(display);
}

void Display::set_pos(Vector &pos) {
    pos_ = Vector(pos);
}

Vector &Display::pos() {
    return pos_;
}

}