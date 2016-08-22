#include "Drawable-ncurses.h"

#ifndef _WIN32
#include "assert.h"
#include "ncurses.h"

static UI8 colorHash[256] = {0};

void init256ColorTable() {
    static UI8 colorSegment[] = {0x00, 0x02f, 0x73, 0x9b, 0xc3, 0xeb, 0xff};

    UI8 idx = 1;
    UI8 cs = colorSegment[idx];
    for (I16 px = 0; px < 256;) {
        if (px <= cs)
            colorHash[px++] = idx - 1;
        else 
            cs = colorSegment[++idx];
    }
}

Drawable::Drawable() {
    I8 succ = init();
    assert(succ == 0);
    init256ColorTable();
}

I8 Drawable::init() {
    initscr();
    noecho();
    curs_set(FALSE);
    if (!has_colors()) {
        printf("must has colors\n");
        return 1;
    }
    start_color();/* Start color */
    for (I32 i = 0; i < COLORS; ++i) {
        init_pair(i, i, i);
    }
    return 0;
}

void Drawable::exitDraw() {
    endwin();
}

void Drawable::clearScreen() {
    clear();
}

UI8 Drawable::get256ColorRGB(I32 rgb) {
    UI8 r = colorHash[rgb >> 16 & 0xff];
    UI8 g = colorHash[rgb >> 8 & 0xff];
    UI8 b = colorHash[rgb & 0xff];
    return r * 36 + g * 6 + b + 16;
}

UI8 Drawable::get256ColorRGB(Color rgb) {
    UI8 r = colorHash[rgb.r];
    UI8 g = colorHash[rgb.g];
    UI8 b = colorHash[rgb.b];
    return r * 36 + g * 6 + b + 16;
}

void Drawable::drawPoint(I32 index, I16 x, I16 y, Color rgb) {
    // init_color(1, rgb.r, rgb.g, rgb.b);
    UI8 color = get256ColorRGB(rgb);
    attron(COLOR_PAIR(color));
    mvprintw(y, x * 2, "  ");
}

void Drawable::buffer2Screen(I16 w, I16 h, Color buffer[BUFFER_SIZE][BUFFER_SIZE]) {
    for (I16 iw = 0; iw < w; ++iw) {
        for (I16 ih = 0; ih < h; ++ih) {
            drawPoint(iw * h + ih, iw, ih, buffer[iw][ih]);
        }
    }
    refresh();
}

void Drawable::getMaxSize(I16& w, I16& h) {
    I16 max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    w = max_x/2;
    h = max_y;
}

#endif