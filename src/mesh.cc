#include "mesh.h"

#include <stdlib.h>

namespace mini3d {

Mesh *Mesh::GenTriangle() {
    Mesh *mesh = new Mesh();
    for (I16 i = 0; i < 3; i++) {
        mesh->indices_.push_back(i);
        Vertex vt;
        vt.p.x = 900.0f;//rand() % 500 * 1.0f;
        vt.p.y = 900.0f;//rand() % 500 * 1.0f;
        vt.p.z = 1000.0f;//rand() % 400 * 1.0f + 550.0f;
        vt.c.r = (I32)rand() * 1.0 / RAND_MAX * 255;
        vt.c.g = (I32)rand() * 1.0 / RAND_MAX * 255;
        vt.c.b = (I32)rand() * 1.0 / RAND_MAX * 255;
        mesh->vertices_.push_back(vt);
    }
    return mesh;
}

Mesh *Mesh::GenByFile(const char *fileName) {
    Mesh *mesh = new Mesh();
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("file not found %s\n", fileName);
        return nullptr;
    }

    while (true) {
        char line_buffer[128];
        int line = fscanf(file, "%s", line_buffer);
        if (line == EOF) {
            break;
        }

        if (strcmp(line_buffer, "v") == 0) { // vertex
            Vertex v;
            v.c = Color::WHITE;
            fscanf(file, "%f %f %f\n", &v.p.x, &v.p.y, &v.p.z);
            mesh->vertices_.push_back(v);
        } else if (strcmp(line_buffer, "vt") == 0) { // uv
            // wait for a good man
        } else if (strcmp(line_buffer, "vn") == 0) { // vertex normal
            // wait for a good man
        } else if (strcmp(line_buffer, "f") == 0) { // vertex normal
            int vi1, vi2, vi3;
            int uvi1, uvi2, uvi3;
            int vni1, vni2, vni3;
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                   &vi1, &uvi1, &vni1, &vi2, &uvi2, &vni2, &vi3, &uvi3, &vni3);
            mesh->indices_.push_back(vi1);
            mesh->indices_.push_back(vi2);
            mesh->indices_.push_back(vi3);
        }
    }
    return mesh;
}

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

void Mesh::Draw(Renderer *r, Matrix &m) {
    for (UI32 i = 0; i < indices_.size(); i += 3) {
        // printf("%d %d %d %d\n", vertices_.size(), indices_.at(i), indices_.at(i + 1), indices_.at(i + 2));
        Vertex vt1 = vertices_.at(indices_.at(i) - 1);
        Vertex vt2 = vertices_.at(indices_.at(i + 1) - 1);
        Vertex vt3 = vertices_.at(indices_.at(i + 2) - 1);
        r->DrawTriangle(vt1, vt2, vt3, m);
    }
}

}  // namespace mini3d
