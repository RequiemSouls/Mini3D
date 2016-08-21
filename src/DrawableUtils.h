#ifndef _DRAWABEL_UTILS_H_
#define _DRAWABEL_UTILS_H_

#include "Drawable-ncurses.h"

class DrawableUtils {
public:
    static DrawableUtils& getInstance();
    void buffer2Screen(int w, int h, Color buffer[2048][2048]);
    void clearScreen();
    void exitDraw();
    void getMaxSize(int& w, int& h);

private:
    DrawableUtils();

private:
    Drawable* _drawable;
};

#endif