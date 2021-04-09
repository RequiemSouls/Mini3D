#include "renderer.h"
#include <assert.h>
#include <cmath>
#include <algorithm>

#define MAX_MESH_COUNT 256
#define px(n) n.get_x()
#define py(n) n.get_y()
#define pz(n) n.get_z()
#define vx(n) n->p.get_x()
#define vy(n) n->p.get_y()
#define vz(n) n->p.get_z()

MINI_NS_BEGIN

Renderer::Renderer(Device &device) : device_(device) {
  device_.GetMaxSize(&width_, &height_);
  camera_.set_ratio(height_ * 1.0f / width_);
  // printf("%d, %d, %f", width_, height_, height_ * 1.0f / width_);
  render_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
  for (int i = 0; i < width_; ++i) {
    render_buffer_[i] = (Color *)calloc(height_, sizeof(Color));
  }

  deep_buffer_ = (F32 **)malloc(sizeof(F32 *) * width_);
  for (int i = 0; i < width_; ++i) {
    deep_buffer_[i] = (F32 *)calloc(height_, sizeof(F32));
  }
}

void Renderer::Clean() {
  static I32 deepRowSize = sizeof(F32) * height_;
  for (int i = 0; i < width_; ++i) {
    memset(deep_buffer_[i], 0, deepRowSize);
  }
  // printf("%f \n", deep_buffer_[100][100]);
  static I32 renderRowSize = sizeof(Color) * height_;
  for (int i = 0; i < width_; ++i) {
    memset(render_buffer_[i], 0, renderRowSize);
  }
}

Renderer::~Renderer() {
  for (int i = 0; i < width_; ++i) {
    free(render_buffer_[i]);
  }
  free(render_buffer_);
}

// screen matrix. convert cvv position(-1, 1) to screen position(-0.5*width,
// 0.5*width)
Matrix Renderer::GetScreenMatrix() {
  Matrix m;
  I32 w = width_ * 0.5;
  I32 h = height_ * 0.5;
  m.set_value(0, 0, w);
  m.set_value(1, 1, h);
  m.set_value(0, 3, w);
  m.set_value(1, 3, h);
  m.set_value(2, 2, 1);
  m.set_value(3, 3, 1);
  return m;
}

// draw a triangle by 3 vertex
void Renderer::DrawTriangle(Vertex &vt1, Vertex &vt2, Vertex &vt3,
                            const Matrix &m) {
  // mvp to clip space
  const Matrix mp = camera_.GetMatrix();
  vt1.p = mp * m * vt1.p;
  vt2.p = mp * m * vt2.p;
  vt3.p = mp * m * vt3.p;
  vt1.p.Homogenize();
  vt2.p.Homogenize();
  vt3.p.Homogenize();

  // 剔除、裁剪
  // Cut(vt1, vt2, vt3);
  Rasterize(vt1, vt2, vt3);
}

void Renderer::Cut(Vertex &vt1, Vertex &vt2, Vertex &vt3) {
  // 顶点位置类型
  char p1Intersection = CheckPoint(vt1.p);
  char p2Intersection = CheckPoint(vt2.p);
  char p3Intersection = CheckPoint(vt3.p);

  // 分类处理
  if (p1Intersection == 0 && p2Intersection == 0 &&
      p3Intersection == 0) { // 三个顶点都在内部，开始光栅
    Rasterize(vt1, vt2, vt3);
  } else if (p1Intersection > 0 && p2Intersection > 0 &&
             p3Intersection > 0) { // 三个顶点都在外面，丢弃. TODO 顶点都在外面但是与cvv相交的情况
  } else { // 一部分在里面一部分在外面
    if (p1Intersection & 1 << 4 || p1Intersection & 1 << 5 ||
        p2Intersection & 1 << 4 || p2Intersection & 1 << 5 ||
        p3Intersection & 1 << 4 ||
        p3Intersection & 1 << 5) { // 只处理深度z超出cvv的情况，因为上下左右超出在光栅的时候会自动处理
      if (p1Intersection > 0 && p2Intersection == 0 &&
          p3Intersection == 0) { // p1 outside. p2 p3 inside.
        Vertex v12 = FindIntersectionVertex(
            p1Intersection, vt1, vt2); // segment p1-p2 intersection point.
        Vertex v13 = FindIntersectionVertex(
            p1Intersection, vt1, vt3); // segment p1-p3 intersection point.
        Rasterize(v12, vt2, vt3);
        Rasterize(v13, v12, vt3);
      } else if (p1Intersection == 0 && p2Intersection > 0 &&
                 p3Intersection == 0) { // p2 outside. p1 p3 inside.
        Vertex v23 = FindIntersectionVertex(
            p2Intersection, vt2, vt3); // segment p1-p3 intersection point.
        Vertex v21 = FindIntersectionVertex(
            p2Intersection, vt2, vt1); // segment p1-p2 intersection point.
        Rasterize(v23, vt3, vt1);
        Rasterize(v21, v23, vt1);
      } else if (p1Intersection == 0 && p2Intersection == 0 &&
                 p3Intersection > 0) { // p3 outside. p1 p2 inside.
        Vertex v31 = FindIntersectionVertex(
            p3Intersection, vt3, vt1); // segment p1-p2 intersection point.
        Vertex v32 = FindIntersectionVertex(
            p3Intersection, vt3, vt2); // segment p1-p3 intersection point.
        Rasterize(v31, vt1, vt2);
        Rasterize(v32, v31, vt2);
      } else if (p1Intersection == 0) { // p1 inside. p2 p3 outside.
        Vertex v21 = FindIntersectionVertex(
            p2Intersection, vt2, vt1); // segment p1-p2 intersection point.
        Vertex v31 = FindIntersectionVertex(
            p3Intersection, vt3, vt1); // segment p1-p3 intersection point.
        Rasterize(v21, v31, vt1);
      } else if (p2Intersection == 0) { // p2 inside. p1 p3 outside.
        Vertex v12 = FindIntersectionVertex(
            p1Intersection, vt1, vt2); // segment p1-p2 intersection point.
        Vertex v32 = FindIntersectionVertex(
            p3Intersection, vt3, vt2); // segment p1-p3 intersection point.
        Rasterize(v12, v32, vt2);
      } else if (p3Intersection == 0) { // p3 inside. p1 p2 outside.
        Vertex v13 = FindIntersectionVertex(
            p1Intersection, vt1, vt3); // segment p1-p2 intersection point.
        Vertex v23 = FindIntersectionVertex(
            p2Intersection, vt2, vt3); // segment p1-p3 intersection point.
        Rasterize(v13, v23, vt3);
      } else {
        printf("error type\n");
      }
    } else { // left right top bottom clip
      Rasterize(vt1, vt2, vt3);
    }
  }
}

char Renderer::CheckPoint(Vector &p) {
  char checkResult = 0;
  if (p.get_x() < -1) { // out left
    checkResult |= 1 << 0;
  }
  if (p.get_x() > 1) { // out right
    checkResult |= 1 << 1;
  }
  if (p.get_y() < -1) { // out top
    checkResult |= 1 << 2;
  }
  if (p.get_y() > 1) { // out bottom
    checkResult |= 1 << 3;
  }
  if (p.get_z() > 1) { // out back
    checkResult |= 1 << 4;
  }
  if (p.get_z() < -1) { // out front
    checkResult |= 1 << 5;
  }
  return checkResult;
}

// 前后平面与线段的交点差值，TODO 实现通用的顶点插值方法
Vertex Renderer::FindIntersectionVertex(char section, Vertex &vt1,
                                        Vertex &vt2) {
  if (section & 1 << 4) {                                            // back
    return (vt1 - vt2) * std::abs((vt2.p.get_z() - 1) / (vt1.p.get_z() - vt2.p.get_z())) + vt2; // interp
  } else if (section & 1 << 5) {                                     // front
    return (vt1 - vt2) * std::abs((vt2.p.get_z() - -1) / (vt1.p.get_z() - vt2.p.get_z())) + vt2;  // interp
  } else {
    return Vertex();
  }
}

void Renderer::Rasterize(Vertex vt1, Vertex vt2, Vertex vt3) {
  const Matrix sm = GetScreenMatrix();
  vt1.p = sm * vt1.p;
  vt2.p = sm * vt2.p;
  vt3.p = sm * vt3.p;
  vt1.p.set_x(floor(vt1.p.get_x() + 0.5));
  vt1.p.set_y(floor(vt1.p.get_y() + 0.5));
  vt2.p.set_x(floor(vt2.p.get_x() + 0.5));
  vt2.p.set_y(floor(vt2.p.get_y() + 0.5));
  vt3.p.set_x(floor(vt3.p.get_x() + 0.5));
  vt3.p.set_y(floor(vt3.p.get_y() + 0.5));
  DrawTriangleScanLine(&vt1, &vt2, &vt3);
  // DrawLineDDA(vt1, vt2);
  // DrawLineDDA(vt2, vt3);
  // DrawLineDDA(vt3, vt1);
  // DrawLineMidVToint(vt1, vt2);
  // DrawLineMidVToint(vt2, vt3);
  // DrawLineMidVToint(vt1, vt3);
  // DrawLineBresenham(vt1.p, vt2.p);
  // DrawLineBresenham(vt2.p, vt3.p);
  // DrawLineBresenham(vt1.p, vt3.p);
}

Vertex Renderer::GetInterp(const Vertex &v1, const Vertex &v2, I32 total) {
  Vertex result;
  if (total == 0) {
    return result;
  }
  result.p.set_x((v1.p.get_x() - v2.p.get_x()) / total);
  result.p.set_y((v1.p.get_y() - v2.p.get_y()) / total);
  result.p.set_z((v1.p.get_z() - v2.p.get_z()) / total);

  result.c.r = (v1.c.r - v2.c.r) / total;
  result.c.g = (v1.c.g - v2.c.g) / total;
  result.c.b = (v1.c.b - v2.c.b) / total;
  return result;
}

Vertex Renderer::OnStep(const Vertex &start, const Vertex &k, I32 step) {
  Vertex result;
  result.p.set_x(I32(start.p.get_x() + k.p.get_x() * step + 0.5));
  result.p.set_y(I32(start.p.get_y() + k.p.get_y() * step + 0.5));
  result.p.set_z(start.p.get_z() + k.p.get_z() * step);

  result.c.r = start.c.r + k.c.r * step;
  result.c.g = start.c.g + k.c.g * step;
  result.c.b = start.c.b + k.c.b * step;
  return result;
}

void Renderer::DrawTriangleScanLine(Vertex *vt1, Vertex *vt2, Vertex *vt3) {
  Vertex *temp = nullptr;
  // sort by vertex y value
  if (vy(vt1) > vy(vt2)) {
    temp = vt1;
    vt1 = vt2;
    vt2 = temp;
  };
  if (vy(vt2) > vy(vt3)) {
    temp = vt2;
    vt2 = vt3;
    vt3 = temp;
  };
  if (vy(vt1) > vy(vt2)) {
    temp = vt1;
    vt1 = vt2;
    vt2 = temp;
  };
  if (vy(vt1) == vy(vt2) && vy(vt1) == vy(vt3))
    return;
  if (vx(vt1) == vx(vt2) && vx(vt1) == vx(vt3))
    return;

  I32 offsetTM = vy(vt3) - vy(vt2); // top-middle y lenght
  I32 offsetMB = vy(vt2) - vy(vt1); // middle-bottom y lenght
  const Vertex &tm =
      GetInterp(*vt3, *vt2, vy(vt3) - vy(vt2)); // top-mid line step size
  const Vertex &tb =
      GetInterp(*vt3, *vt1, vy(vt3) - vy(vt1)); // top-bottom line step size
  const Vertex &mb =
      GetInterp(*vt2, *vt1, vy(vt2) - vy(vt1)); // mid-bottom line step size
  for (I32 step = 1; step <= offsetMB;
       step++) { // step by step draw top-mid triangle horizontal line
    Vertex lv = OnStep(*vt1, mb, step); // step mid-bottom line
    if (lv.p.get_y() < 0 || lv.p.get_y() > height_)
      continue;
    Vertex rv = OnStep(*vt1, tb, step); // step top-bottom line
    if (rv.p.get_y() < 0 || rv.p.get_y() > height_)
      continue;
    DrawHorizontalLine(lv, rv);
  }

  for (I32 step = 0; step <= offsetTM;
       step++) { // step by step draw mid-bottom triangle horizontal line
    Vertex lv = OnStep(*vt3, tm, -step); // step top-mid line
    if (lv.p.get_y() < 0 || lv.p.get_y() > height_)
      continue;                                 // out screen
    Vertex rv = OnStep(*vt3, tb, -step); // step tom-bottom line
    if (rv.p.get_y() < 0 || rv.p.get_y() > height_)
      continue; // out screen
    DrawHorizontalLine(lv, rv);
  }
}

void Renderer::DrawHorizontalLine(Vertex &lv, Vertex &rv) {
  lv.p.set_x(std::min(std::max(lv.p.get_x(), 0.0f), (float)width_)); // clip inside screen
  rv.p.set_x(std::min(std::max(rv.p.get_x(), 0.0f), (float)width_)); // clip inside screen
  I32 gap = std::abs(lv.p.get_x() - rv.p.get_x());
  const Vertex &k = GetInterp(lv, rv, gap); // horizontal step size
  for (int i = 0; i <= gap; ++i) {          // step by step horizontal pixel
    Vertex point = OnStep(rv, k, i);
    DrawPixel(point.p.get_x(), point.p.get_y(), point.p.get_z(), point.c);
  }
}

void Renderer::DrawLineBresenham(Vector &p1, Vector &p2) {
  F32 dx, dy, dz, absdx, absdy, absdz;
  dx = px(p1) - px(p2);
  dy = py(p1) - py(p2);
  dz = pz(p1) - pz(p2);
  absdx = std::abs(dx);
  absdy = std::abs(dy);
  absdz = std::abs(dz);
  if (absdx < 0.00001 && absdy < 0.00001) {
    return;
  }
  I32 x = px(p1), y = py(p1);
  F32 z = pz(p1);
  F32 d = 0;
  I32 stepx = absdx == 0 ? 0 : dx / absdx;
  I32 stepy = absdy == 0 ? 0 : dy / absdy;
  if (absdx >= absdy) {
    I32 total = absdx;
    F32 stepz = dz / total;
    F32 k = std::abs(dy / dx);
    for (I32 i = 0; i < total; ++i) {
      x -= stepx;
      d += k;
      if (d > 1) {
        y -= stepy;
        d -= 1;
      }
      z -= stepz;
      DrawPixel(x, y, z, Color::WHITE);
    }
  } else if (absdx < absdy) {
    I32 total = absdy;
    F32 stepz = dz / total;
    F32 k = std::abs(dx / dy);
    for (I32 i = 0; i < total; ++i) {
      y -= stepy;
      d += k;
      if (d > 1) {
        x -= stepx;
        d -= 1;
      }
      z -= stepz;
      DrawPixel(x, y, z, Color::WHITE);
    }
  }
}

void Renderer::DrawPixel(I32 x, I32 y, F32 z, const Color &c) {
  if (x >= width_ || y >= height_) return;
  if (deep_buffer_[x][y] > z - 1) {
    deep_buffer_[x][y] = z - 1;
    render_buffer_[x][y] = c;
  }
}

void Renderer::Buffer2Screen() const { device_.Buffer2Screen(render_buffer_); }

Camera &Renderer::camera() { return camera_; }

MINI_NS_END
