#include "renderer.h"
#include <assert.h>
#include <cmath>

#define MAX_MESH_COUNT 256

namespace mini3d {

Renderer::Renderer(Device &device) : device_(device) {
    device_.GetMaxSize(&width_, &height_);
    camera_.set_ratio(height_ * 1.0f / width_);
    printf("%d, %d, %f", width_, height_, height_ * 1.0f / width_);
    render_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
    for (int i = 0; i < width_; ++i) {
        render_buffer_[i] = (Color *)calloc(height_, sizeof(Color));
    }
    Buffer2Screen();
}

Renderer::~Renderer() {
    for (int i = 0; i < width_; ++i) {
        free(render_buffer_[i]);
    }
    free(render_buffer_);
}

void Renderer::Render() {
    static I32 sizePerBuffer = sizeof(Color) * height_;
    for (int i = 0; i < width_; ++i) {
        memset(render_buffer_[i], 0, sizePerBuffer);
    }
}

void Renderer::DrawTriangle(Vertex &vt1, Vertex &vt2,
                            Vertex &vt3, const Matrix &m) {
    Matrix mp = camera_.GetMatrix();
    vt1.p = mp * m * vt1.p;
    vt2.p = mp * m * vt2.p;
    vt3.p = mp * m * vt3.p;
    vt1.p.Homogenize();
    vt2.p.Homogenize();
    vt3.p.Homogenize();
    Rasterize(vt1, vt2, vt3);
}

void Renderer::Rasterize(Vertex& vt1, Vertex& vt2, Vertex& vt3) {
    if (vt1.p.get_x() > -1 && vt1.p.get_x() < 1 && vt1.p.get_y() > -1 && vt1.p.get_y() < 1 &&
    vt2.p.get_x() > -1 && vt2.p.get_x() < 1 && vt2.p.get_y() > -1 && vt2.p.get_y() < 1 &&
        vt3.p.get_x() > -1 && vt3.p.get_x() < 1 && vt3.p.get_y() > -1 && vt3.p.get_y() < 1 )
    {
		DrawTriangleScanLine(&vt1, &vt2, &vt3);
        //DrawLineDDA(vt1, vt2);
        //DrawLineDDA(vt2, vt3);
        //DrawLineDDA(vt3, vt1);
		//DrawLineMidVToint(vt1, vt2);
		//DrawLineMidVToint(vt2, vt3);
		//DrawLineMidVToint(vt1, vt3);
		DrawLineBresenham(vt1.p, vt2.p);
		DrawLineBresenham(vt2.p, vt3.p);
		DrawLineBresenham(vt1.p, vt3.p);
    }
}

void Renderer::DrawTriangleScanLine(Vertex* vt1, Vertex* vt2, Vertex* vt3) {
    Vertex* temp = nullptr;
    // sort
    if (vt1->p.get_y() > vt2->p.get_y()) { temp = vt1; vt1 = vt2; vt2 = temp; };
    if (vt2->p.get_y() > vt3->p.get_y()) { temp = vt2; vt2 = vt3; vt3 = temp; };
    if (vt1->p.get_y() > vt2->p.get_y()) { temp = vt1; vt1 = vt2; vt2 = temp; };
	if (vt1->p.get_y() == vt2->p.get_y() && vt1->p.get_y() == vt3->p.get_y()) return;
	if (vt1->p.get_x() == vt2->p.get_x() && vt1->p.get_x() == vt3->p.get_x()) return;

    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    I32 tx, ty, mx, my, bx, by;
    tx = vt3->p.get_x() * w + w;
    ty = vt3->p.get_y() * h + h;
    mx = vt2->p.get_x() * w + w;
    my = vt2->p.get_y() * h + h;
    bx = vt1->p.get_x() * w + w;
    by = vt1->p.get_y() * h + h;
    UI8 tr = vt3->c.r, tg = vt3->c.g, tb = vt3->c.b;
    UI8 mr = vt2->c.r, mg = vt2->c.g, mb = vt2->c.b;
    UI8 br = vt1->c.r, bg = vt1->c.g, bb = vt1->c.b;
    I32 obty = ty - by;
    I32 obmy = my - by;
    I32 omty = ty - my;
    F32 btk = obty == 0 ? 2147483647 : (tx - bx) * 1.0f / obty;
    F32 bmk = obmy == 0 ? 2147483647 : (mx - bx) * 1.0f / obmy;
    F32 mtk = omty == 0 ? 2147483647 : (tx - mx) * 1.0f / omty;
    bool leftT = true;
    if (bmk > btk) leftT = false;
    I32 lx = 0, rx = 0;
    Color lc, rc;
    for (I32 step = 0, y = by; step <= obmy; y++, step++) {
        if (leftT) {
            lx = I32(bx + bmk * step);
            rx = I32(bx + btk * step);
            lc.r = GetInterp(br, mr, obmy, step);
            lc.g = GetInterp(bg, mg, obmy, step);
            lc.b = GetInterp(bb, mb, obmy, step);
            rc.r = GetInterp(br, tr, obty, step);
            rc.g = GetInterp(bg, tg, obty, step);
            rc.b = GetInterp(bb, tb, obty, step);
        } else {
            lx = I32(bx + btk * step);
            rx = I32(bx + bmk * step);
            lc.r = GetInterp(br, tr, obty, step);
            lc.g = GetInterp(bg, tg, obty, step);
            lc.b = GetInterp(bb, tb, obty, step);
            rc.r = GetInterp(br, mr, obmy, step);
            rc.g = GetInterp(bg, mg, obmy, step);
            rc.b = GetInterp(bb, mb, obmy, step);
        }
        DrawHorizontalLine(y, lx, rx, lc, rc);
    }
    for (I32 step = 0, y = ty; step <= omty; y--, step++) {
        if (leftT) {
            lx = I32(tx - mtk * step);
            rx = I32(tx - btk * step);
            lc.r = GetInterp(mr, tr, omty, step);
            lc.g = GetInterp(mg, tg, omty, step);
            lc.b = GetInterp(mb, tb, omty, step);
            rc.r = GetInterp(br, tr, obty, step);
            rc.g = GetInterp(bg, tg, obty, step);
            rc.b = GetInterp(bb, tb, obty, step);
        } else {
            lx = I32(tx - btk * step);
            rx = I32(tx - mtk * step);
            rx = I32(tx - btk * step);
            lc.r = GetInterp(br, tr, obty, step);
            lc.g = GetInterp(bg, tg, obty, step);
            lc.b = GetInterp(bb, tb, obty, step);
            rc.r = GetInterp(mr, tr, omty, step);
            rc.g = GetInterp(mg, tg, omty, step);
            rc.b = GetInterp(mb, tb, omty, step);
        }
        DrawHorizontalLine(y, lx, rx, lc, rc);
    }
}

I32 Renderer::GetInterp(I32 lv, I32 rv, I32 offset, I32 step) {
    return offset == 0 ? 2147483647 : lv + (rv - lv) * 0.1f / offset * step;
}

void Renderer::DrawHorizontalLine(I32 y, I32 lx, I32 rx, Color lc, Color rc) {
    I32 offx = rx - lx;
    F32 rstep = offx == 0 ? 0 : (rc.r - lc.r) * 1.0f / offx;
    F32 gstep = offx == 0 ? 0 : (rc.g - lc.g) * 1.0f / offx;
    F32 bstep = offx == 0 ? 0 : (rc.b - lc.b) * 1.0f / offx;
    for (I32 x = lx, step = 0; x <= rx; step++, x++) {
        DrawPixel(x, y, Color(lc.r + rstep * step, lc.g + gstep * step, lc.b + bstep * step));
    }
}

void Renderer::DrawLineDDA(Vector& p1, Vector& p2) {
    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    F32 x1, y1, x2, y2;
    x1 = p1.get_x() * w + w;
    x2 = p2.get_x() * w + w;
    y1 = -p1.get_y() * h + h;
    y2 = -p2.get_y() * h + h;

    I32 total = 0;
    F32 dx, dy, absdx, absdy;
    F32 kx, ky, x, y;
    dx = x1 - x2;
    dy = y1 - y2;
    absdx = std::abs(dx);
    absdy = std::abs(dy);

    if (absdx >= absdy) {
        total = absdx;
    } else if (absdy > absdx) {
        total = absdy;
    }
    if (total == 0) {
        return;
    }

    x = x1;
    y = y1;
    kx = dx / total;
    ky = dy / total;
    Color rc = Color::WHITE;
    for (I32 i = 0; i < total; ++i) {
        x -= kx;
        y -= ky;
        DrawPixel(x, y, rc);
    }
}

void Renderer::DrawLineMidPoint(Vector& p1, Vector& p2) {
    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    I32 x1, y1, x2, y2;
    x1 = p1.get_x() * w + w;
    x2 = p2.get_x() * w + w;
    y1 = p1.get_y() * h + h;
    y2 = p2.get_y() * h + h;

    F32 dx, dy, absdx, absdy;
    dx = x1 - x2;
    dy = y1 - y2;
    absdx = std::abs(dx);
    absdy = std::abs(dy);
    if (absdx < 0.00001 && absdy < 0.00001) {
        return;
    }

    I32 x, y;
    x = x1 + 0.5;
    y = y1 + 0.5;
    I32 total = 0;
    I32 kx = absdx < 0.00001 ? 0 : dx/absdx;
    I32 ky = absdy < 0.00001 ? 0 : dy/absdy;
    F32 a, b, c;
    a = y1 - y2;
    b = x2 - x1;
    c = x1 * y2 - x2 * y1;
    if (absdx > absdy) {
        total = absdx;
        for (I32 i = 0; i < total; ++i) {
            x -= kx;
			F32 temp = a * x + b * (y - ky * 0.5) + c;
			temp *= kx;
            if (temp < 0 && ky > 0) {
                y--;
			} else if (temp > 0 && ky < 0) {
				y++;
			}
            DrawPixel(x, height_ - y, Color::WHITE);
        }
    } else if (absdy > absdx) {
        total = absdy;
        for (I32 i = 0; i < total; ++i) {
            y -= ky;
			F32 temp = a * (x - kx * 0.5) + b * y + c;
			temp *= ky * -1;
            if (temp < 0 && kx > 0) {
                x--;
			} else if (temp > 0 && kx < 0) {
				x++;
			}
            DrawPixel(x, height_ - y, Color::WHITE);
        }
    }
}

void Renderer::DrawLineBresenham(Vector& p1, Vector& p2) {
    I32 w = width_ * 0.5;
    I32 h = height_ * 0.5;
    I32 x1, y1, x2, y2;
    x1 = p1.get_x() * w + w;
    x2 = p2.get_x() * w + w;
    y1 = p1.get_y() * h + h;
    y2 = p2.get_y() * h + h;

	F32 dx, dy, absdx, absdy;
	dx = x1 - x2;
	dy = y1 - y2;
	absdx = std::abs(dx);
	absdy = std::abs(dy);
	if (absdx < 0.00001 && absdy < 0.00001) {
		return;
	}
	I32 x = x1, y = y1;
	F32 d = 0;
	I32 stepx = absdx == 0 ? 0 : dx / absdx;
	I32 stepy = absdy == 0 ? 0 : dy / absdy;
	if (absdx >= absdy) {
		I32 total = absdx;
		F32 k = std::abs(dy / dx);
		for (I32 i = 0; i < total; ++i) {
			x -= stepx;
			d += k;
			if (d > 1) {
				y -= stepy;
				d -= 1;
			}
			DrawPixel(x, y, Color::WHITE);
		}
	} else if (absdx < absdy) {
		I32 total = absdy;
		F32 k = std::abs(dx / dy);
		for (I32 i = 0; i < total; ++i) {
			y -= stepy;
			d += k;
			if (d > 1) {
				x -= stepx;
				d -= 1;
			}
			DrawPixel(x, y, Color::WHITE);
		}
	}
}

void Renderer::DrawPixel(I32 x, I32 y, Color c) {
    render_buffer_[x][y] = c;
}

void Renderer::Buffer2Screen() const { device_.Buffer2Screen(render_buffer_); }

Camera& Renderer::camera() { return camera_; }

}  // namespace mini3d