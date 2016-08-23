#ifndef _WIN32

#include "device.h"

#include <time.h>

#include <chrono>
#include <thread>
#include "assert.h"

#define CURSOR_WIDTH 1

#define GET_256_COLOR(r, g, b) \
    (colorHash_[r] * 36 + colorHash_[g] * 6 + colorHash_[b] + 16)

Device &Device::GetInstance() {
    static Device instance;
    return instance;
}

Device::Device() {
    I8 succ = Init();
    assert(succ == 0);    
    Init256ColorTable();
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

    I16 offsetX = 4;
    I16 offsetY = 4;    
    height_ = LINES - offsetY * 2;
    width_ = COLS - offsetX * 2;
    win_ = newwin(height_, width_, offsetX, offsetY);
    
    height_ -= 2;
    width_ -= 2;

    width_ /= CURSOR_WIDTH;

    return 0;
}

void Device::SetLoopEvent(LoopEvent &&le) {
    loopEvent_ = std::move(le);
}

I8 Device::Loop() {
    I16 clocks_per_ms = CLOCKS_PER_SEC / 1000.0;
    const I32 FRAME_TIME = 40*clocks_per_ms; //40ms fps=25

    clock_t curDT = 0;
    clock_t mpfDT = FRAME_TIME;

    
    while (true) {
        curDT = clock();
        F32 FPS = CLOCKS_PER_SEC / (1.0*mpfDT);
        if (FPS > 60.0f) FPS = 60.0f;

        printf("FPS: %.1f/%.1fms\n", FPS, mpfDT / (1.0*clocks_per_ms));

        if (loopEvent_ != nullptr) {
            loopEvent_();
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

void Device::Buffer2Screen(Color buffer[BUFFER_SIZE][BUFFER_SIZE]) {
    wclear(win_);
    wattron(win_, COLOR_PAIR(GET_256_COLOR(0xff, 0xff, 0xff)));
    //wborder(win_, '|', '|', '-', '-', '-', '-', '-', '-');
    wborder(win_, '-', '-', '-', '-', '-', '-', '-', '-');
    for (I16 x = 0; x < width_; ++x) {
        for (I16 y = 0; y < height_; ++y) {
            Color &rgb = buffer[x][y];
            wattron(win_, COLOR_PAIR(GET_256_COLOR(rgb.r, rgb.g, rgb.b)));
            mvwprintw(win_, 1 + y, 1 + x * CURSOR_WIDTH, " ");
        }
    }
    wrefresh(win_);
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
            colorHash_[px++] = idx - 1;
        else
            cs = colorSegment[++idx];
    }
}

#endif