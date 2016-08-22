#include "Drawable-ncurses.h"

#ifndef _WIN32
#include "assert.h"
#include "ncurses.h"

#define GET_256_COLOR(r, g, b) (colorHash[r] * 36 + colorHash[g] * 6 + colorHash[b] + 16)

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

void Drawable::buffer2Screen(I16 w, I16 h, Color buffer[BUFFER_SIZE][BUFFER_SIZE]) {
    for (I16 x = 0; x < w; ++x) {
        for (I16 y = 0; y < h; ++y) {
            Color& rgb = buffer[x][y];
            attron(COLOR_PAIR(GET_256_COLOR(rgb.r, rgb.g ,rgb.b)));
            mvprintw(y, x * 2, "  ");
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