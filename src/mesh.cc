#include "mesh.h"

Mesh *Mesh::GenTriangle() {
    Mesh *mesh = new Mesh();
    for (I16 i = 0; i < 3; i++) {
        mesh->indexArray_[i] = i;
        Vertex *vt = new Vertex();
        vt->p.x = rand() * 1.0 / RAND_MAX;
        vt->p.y = rand() * 1.0 / RAND_MAX;
        vt->p.z = rand() * 1.0 / RAND_MAX;
        vt->c.r = rand() * 255.0 / RAND_MAX;
        vt->c.g = rand() * 255.0 / RAND_MAX;
        vt->c.b = rand() * 255.0 / RAND_MAX;
        mesh->vertexArray_[i] = vt;
    }
    mesh->tCount_ = 1;

    return mesh;
}

Mesh *Mesh::GenByFile(const char *fileName) {
    // wait for good man
    return nullptr;
}

Mesh::Mesh() {
    tCount_ = 0;
    vertexArray_ = (Vertex **)malloc(sizeof(Vertex *) * 256);
}

Mesh::~Mesh() {
    for (I32 i = 0; i < tCount_; ++i) {
        delete vertexArray_[indexArray_[i * 3]];
        delete vertexArray_[indexArray_[i * 3 + 1]];
        delete vertexArray_[indexArray_[i * 3 + 2]];
    }
    free(vertexArray_);
}

void Mesh::Draw(Renderer *r, Matrix m) {
    for (I32 i = 0; i < tCount_; ++i) {
        // multiply Matrix
        Vertex *vt1 = vertexArray_[indexArray_[i * 3]];      // * m
        Vertex *vt2 = vertexArray_[indexArray_[i * 3 + 1]];  // * m
        Vertex *vt3 = vertexArray_[indexArray_[i * 3 + 2]];  // * m
        r->DrawTriangle(vt1, vt2, vt3);
    }
}