#include "Drawable-ncurses.h"
#include "Types.h"

#ifndef _WIN32
#include "assert.h"
#include "ncurses.h"

static UI8 colorHash[256] = {0};

void init256ColorTable(){
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
    int succ = init();
    assert(succ == 0);
    init256ColorTable();
}

int Drawable::init() {
    initscr();
    noecho();
    curs_set(FALSE);
    if (!has_colors()) {
        printf("must has colors\n");
        return 1;
    }
    start_color();/* Start color */
    for (int i = 0; i < COLORS; ++i) {
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

UI8 Drawable::get256ColorRGB(int rgb) {
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

void Drawable::drawPoint(int index, int x, int y, Color rgb) {
    // init_color(1, rgb.r, rgb.g, rgb.b);
    UI8 color = get256ColorRGB(rgb);
    attron(COLOR_PAIR(color));
    mvprintw(y, x * 2, "  ");
}

void Drawable::buffer2Screen(int w, int h, Color buffer[2048][2048]) {
    for (int iw = 0; iw < w; ++iw) {
        for (int ih = 0; ih < h; ++ih) {
            drawPoint(iw * h + ih, iw, ih, buffer[iw][ih]);
        }
    }
    refresh();
}

void Drawable::getMaxSize(int& w, int& h) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    w = max_x/2;
    h = max_y;
}

#endif