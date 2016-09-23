#include "mesh.h"

#include <stdlib.h>

namespace mini3d {

Mesh Mesh::GenTriangle() {
    Mesh mesh;
    for (I16 i = 0; i < 3; i++) {
        mesh.indices_.push_back(i);
        Vertex vt;
        vt.p.set_x(900.0f);   // rand() % 500 * 1.0f;
        vt.p.set_y(900.0f);   // rand() % 500 * 1.0f;
        vt.p.set_z(1000.0f);  // rand() % 400 * 1.0f + 550.0f;
        vt.c.r = (I32)rand() * 1.0 / RAND_MAX * 255;
        vt.c.g = (I32)rand() * 1.0 / RAND_MAX * 255;
        vt.c.b = (I32)rand() * 1.0 / RAND_MAX * 255;
        mesh.vertices_.push_back(vt);
    }
    return mesh;
}

Mesh Mesh::GenByFile(const char *fileName) {
    Mesh mesh;
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("file not found %s\n", fileName);
        return mesh;
    }

    while (true) {
        char line_buffer[128];
        int line = fscanf(file, "%s", line_buffer);
        if (line == EOF) {
            break;
        }

        if (strcmp(line_buffer, "v") == 0) {  // vertex
            Vertex v;
            v.c = Color::WHITE;
            F32 x, y, z;
            int ret = fscanf(file, "%f %f %f\n", &x, &y, &z);
            v.p = {x, y, z};
            if (ret == EOF || ret != 3) {
                break;
            }
            mesh.vertices_.push_back(v);
        } else if (strcmp(line_buffer, "vt") == 0) {  // uv
            // wait for a good man
        } else if (strcmp(line_buffer, "vn") == 0) {  // vertex normal
            // wait for a good man
        } else if (strcmp(line_buffer, "f") == 0) {  // face index
            int vi1, vi2, vi3;
            int uvi1, uvi2, uvi3;
            int vni1, vni2, vni3;
            // int ret = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vi1, &uvi1,
            //                  &vni1, &vi2, &uvi2, &vni2, &vi3, &uvi3, &vni3);
            // if (ret == EOF || ret != 9) {
            //     break;
            // }
            int ret = fscanf(file, "%d//%d %d//%d %d//%d\n", &vi1, &vni1, &vi2, &vni2, &vi3, &vni3);
            if (ret == EOF || ret != 6) {
                break;
            }
            mesh.indices_.push_back(vi1);
            mesh.indices_.push_back(vi2);
            mesh.indices_.push_back(vi3);
        }
    }
    return mesh;
}

void Mesh::Draw(Renderer *r, const Matrix &m) {
    for (UI32 i = 0; i < indices_.size(); i += 3) {
        // printf("%d %d %d %d\n", vertices_.size(), indices_.at(i),
        // indices_.at(i + 1), indices_.at(i + 2));
        Vertex vt1 = vertices_.at(indices_.at(i) - 1);
        Vertex vt2 = vertices_.at(indices_.at(i + 1) - 1);
        Vertex vt3 = vertices_.at(indices_.at(i + 2) - 1);
        r->DrawTriangle(vt1, vt2, vt3, m);
    }
}

}  // namespace mini3d
