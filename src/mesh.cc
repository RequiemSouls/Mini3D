#include "mesh.h"

namespace mini3d {

Mesh *Mesh::GenTriangle() {
    Mesh *mesh = new Mesh();
    for (I16 i = 0; i < 3; i++) {
        mesh->index_array_[i] = i;
        Vertex *vt = new Vertex();
        vt->p.x = rand() * 1.0 / RAND_MAX;
        vt->p.y = rand() * 1.0 / RAND_MAX;
        vt->p.z = rand() * 1.0 / RAND_MAX;
        vt->c.r = rand() * 255.0 / RAND_MAX;
        vt->c.g = rand() * 255.0 / RAND_MAX;
        vt->c.b = rand() * 255.0 / RAND_MAX;
        mesh->vertex_array_[i] = vt;
    }
    mesh->count_ = 1;
    return mesh;
}

Mesh *Mesh::GenByFile(const char *fileName) {
    // wait for good man
    return nullptr;
}

Mesh::Mesh() {
    count_ = 0;
    vertex_array_ = (Vertex **)malloc(sizeof(Vertex *) * 256);
}

Mesh::~Mesh() {
    for (I32 i = 0; i < count_; ++i) {
        delete vertex_array_[index_array_[i * 3]];
        delete vertex_array_[index_array_[i * 3 + 1]];
        delete vertex_array_[index_array_[i * 3 + 2]];
    }
    free(vertex_array_);
}

void Mesh::Draw(Renderer *r, Matrix m) {
    for (I32 i = 0; i < count_; ++i) {
        // multiply Matrix
        Vertex *vt1 = vertex_array_[index_array_[i * 3]];      // * m
        Vertex *vt2 = vertex_array_[index_array_[i * 3 + 1]];  // * m
        Vertex *vt3 = vertex_array_[index_array_[i * 3 + 2]];  // * m
        r->DrawTriangle(vt1, vt2, vt3);
    }
}

}  // namespace mini3d
