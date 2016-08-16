#include "stdlib.h"
#include "Mesh.h"

Mesh* Mesh::genTriangle() {
    Mesh* mesh = new Mesh();
    for (int i = 0; i < 3; i++) {
        mesh->indexArray[i] = i;
        Vertex* vt = new Vertex();
        vt->p.x = rand() * 0.1 / RAND_MAX;
        vt->p.y = rand() * 0.1 / RAND_MAX;
        vt->p.z = rand() * 0.1 / RAND_MAX;
        vt->c.r = 255;
        vt->c.g = 255;
        vt->c.b = 255;
        mesh->vertexArray[i] = vt;
    }
    mesh->tCount = 1;

    return mesh;
}

Mesh* Mesh::genByFile(const char* fileName) {
    // wait for good man
    return nullptr;
}

Mesh::Mesh() {
    // init mesh
    tCount = 0;
    vertexArray = (Vertex**)malloc(sizeof(Vertex*) * 256);
}

Mesh::~Mesh() {
    // delete mesh
    for (int i = 0; i < tCount; ++i) {
        delete vertexArray[indexArray[i*3]];
        delete vertexArray[indexArray[i*3+1]];
        delete vertexArray[indexArray[i*3+2]];
    }
    free(vertexArray);
}

void Mesh::draw(Renderer* r, Matrix m) {
    for (int i = 0; i < tCount; ++i) {
        // multiply Matrix
        Vertex* vt1 = vertexArray[indexArray[i*3]]; // * m
        Vertex* vt2 = vertexArray[indexArray[i*3+1]]; // * m
        Vertex* vt3 = vertexArray[indexArray[i*3+2]]; // * m
        r->drawTriangle(vt1, vt2, vt3);
    }
}