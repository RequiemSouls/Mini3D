#ifndef _DRAWABLE_NCURSES_H_
#define _DRAWABLE_NCURSES_H_

#include "geometry.h"
class Drawable{
public:
    Drawable();
    void drawPoint(int index, int x, int y, Color rgb);
    void buffer2Screen(int w, int h, Color buffer[2048][2048]);
    void clearScreen();
    void exitDraw();
    void getMaxSize(int& w, int& h);

private:
    int init();
    unsigned char get256Color(unsigned char rgb);
    unsigned char get256ColorRGB(int rgb);
    unsigned char get256ColorRGB(Color rgb);
};

#endif