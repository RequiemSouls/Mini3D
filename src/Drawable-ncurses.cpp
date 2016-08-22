#include "Drawable-ncurses.h"
#include "Types.h"

#ifndef _WIN32
#include "assert.h"
#include "ncurses.h"

Drawable::Drawable() {
    int succ = init();
    assert(succ == 0);
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

UI8 Drawable::get256ColorIndex(UI8 rgb) {
    //static short colorLevel[6] = {0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff};
    static UI8 colorSegment[] = {0x00, 0x02f, 0x73, 0x9b, 0xc3, 0xeb, 0xff};

    for (UI8 i = 1; i < 7; ++i) {
        if (rgb <= colorSegment[i]) {
            return i-1;
        }
    }
    return 0;
}

UI8 Drawable::get256ColorRGB(int rgb) {
    UI8 r = get256ColorIndex(rgb >> 16 & 0xff);
    UI8 g = get256ColorIndex(rgb >> 8 & 0xff);
    UI8 b = get256ColorIndex(rgb & 0xff);
    return r * 36 + g * 6 + b + 16;
}

UI8 Drawable::get256ColorRGB(Color rgb) {
    UI8 r = get256ColorIndex(rgb.r);
    UI8 g = get256ColorIndex(rgb.g);
    UI8 b = get256ColorIndex(rgb.b);
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