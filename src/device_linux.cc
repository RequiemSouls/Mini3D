#ifndef _WIN32

#include "device.h"

#include <time.h>
#include <assert.h>
#include <ncurses.h>

#include <chrono>
#include <thread>

#define CURSOR_WIDTH 2
#define CURSOR_STRING "  "
#define BOTTOM_OFFSET 1
#define FRAME_TIME 16

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
    height_ = LINES - BOTTOM_OFFSET;
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
    const I32 frameTime = FRAME_TIME * clocks_per_ms;

    clock_t curDT = 0;
    clock_t mpfDT = frameTime;

    while (true) {
        frame_count_++;
        curDT = clock();
        fps_ = CLOCKS_PER_SEC / (1.0*mpfDT);
        if (fps_ > 60.0f) fps_ = 60.0f;
        render_time_ = mpfDT / (1.0*clocks_per_ms);

        if (loop_event_ != nullptr) {
            loop_event_();
        }
        mpfDT = clock() - curDT;

        if (mpfDT < frameTime) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds((frameTime - mpfDT)/clocks_per_ms));
        }
    }
    return 0;
}

void Device::ExitDraw() { endwin(); }

void Device::Buffer2Screen(Color **buffer) {
    int count = 0;
    for (I16 x = 0; x < width_; ++x) {
        for (I16 y = 0; y < height_; ++y) {
            Color &rgb = buffer[x][y];
            if (rgb.r != 0) {
                count++;
            }
            if (screen_buffer_[x][y] != rgb) {
                screen_buffer_[x][y] = rgb;
                attron(COLOR_PAIR(GET_256_COLOR(rgb.r, rgb.g, rgb.b)));
                mvprintw(y, x * CURSOR_WIDTH, CURSOR_STRING);
            }
        }
    }
    attroff(COLOR_PAIR(GET_256_COLOR(0xff, 0xff, 0xff)));
    mvprintw(height_, 0, "Mesh Count: %d FPS: %.1f/%.1fms Frame Count: %d point : %d %s\n",
             mesh_count_, fps_, render_time_, frame_count_, count, log_);
    refresh();
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

void Device::set_log(I32 num) {
    char str[16] = {0};

    sprintf(str, "%d" , num);
    set_log(str);
}

}  // namespace mini3d

#endif
