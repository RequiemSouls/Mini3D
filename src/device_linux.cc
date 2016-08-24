#ifndef _WIN32

#include "device.h"

#include <time.h>
#include <chrono>
#include <thread>
#include "assert.h"

#define CURSOR_WIDTH 1

#define GET_256_COLOR(r, g, b) \
    (color_hash_[r] * 36 + color_hash_[g] * 6 + color_hash_[b] + 16)

namespace mini3d {

Device &Device::GetInstance() {
    static Device instance;
    return instance;
}

Device::Device() {
    I8 succ = Init();
    assert(succ == 0);    
    Init256ColorTable();
}

Device::~Device() {
    for (int i = 0; i < width_; ++i) {
        free(screen_buffer_[i]);
    }
    free(screen_buffer_);
}

I8 Device::Init() {
    initscr();
    noecho();
    curs_set(FALSE);
    if (!has_colors()) {
        printf("must has colors\n");
        return 1;
    }
    start_color(); /* Start color */
    for (I32 i = 0; i < COLORS; ++i) {
        init_pair(i, i, i);
    }   
    height_ = LINES - bottom_offset_;
    width_ = COLS;
    width_ /= CURSOR_WIDTH;

    screen_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
    for (int i = 0; i < width_; ++i) {
        screen_buffer_[i] = (Color *)malloc(height_ * sizeof(Color));
        memset(screen_buffer_[i], 0xff, height_ * sizeof(Color));
    }
    return 0;
}

void Device::SetLoopEvent(LoopEvent &&le) {
    loop_event_ = std::move(le);
}

I8 Device::Loop() {
    I16 clocks_per_ms = CLOCKS_PER_SEC / 1000.0;
    const I32 FRAME_TIME = 16*clocks_per_ms; //16ms fps=60

    clock_t curDT = 0;
    clock_t mpfDT = FRAME_TIME;

    while (true) {
        curDT = clock();
        F32 FPS = CLOCKS_PER_SEC / (1.0*mpfDT);
        if (FPS > 60.0f) FPS = 60.0f;        
        attroff(COLOR_PAIR(GET_256_COLOR(0xff, 0xff, 0xff)));
        mvprintw(height_, 0, "MESH COUNT: %d FPS: %.1f/%.1fms\n", mesh_count_, FPS, mpfDT / (1.0*clocks_per_ms));
        
        if (loop_event_ != nullptr) {
            loop_event_();
        }
        mpfDT = clock() - curDT;

        if (mpfDT < FRAME_TIME) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds((FRAME_TIME - mpfDT)/clocks_per_ms));
        }
    }
    return 0;
}

void Device::ExitDraw() { endwin(); }

void Device::Buffer2Screen(Color **buffer) {
    for (I16 x = 0; x < width_; ++x) {
        for (I16 y = 0; y < height_; ++y) {
            Color &rgb = buffer[x][y];
            if (screen_buffer_[x][y] != rgb) {
                screen_buffer_[x][y] = rgb;
                attron(COLOR_PAIR(GET_256_COLOR(rgb.r, rgb.g, rgb.b)));
                mvprintw(y, x * CURSOR_WIDTH, " ");                
            }
        }
    }
    refresh();
}

void Device::GetMaxSize(I16 &w, I16 &h) {
    w = width_;
    h = height_;
}

void Device::Init256ColorTable() {
    UI8 colorSegment[] = {0x00, 0x02f, 0x73, 0x9b, 0xc3, 0xeb, 0xff};

    UI8 idx = 1;
    UI8 cs = colorSegment[idx];
    for (I16 px = 0; px < 256;) {
        if (px <= cs)
            color_hash_[px++] = idx - 1;
        else
            cs = colorSegment[++idx];
    }
}

}  // namespace mini3d

#endif
