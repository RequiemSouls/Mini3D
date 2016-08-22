#ifndef _DRAWABEL_UTILS_H_
#define _DRAWABEL_UTILS_H_

#include "Drawable-ncurses.h"

class DrawableUtils {
   public:
    static DrawableUtils &getInstance();
    void buffer2Screen(I16 w, I16 h, Color buffer[BUFFER_SIZE][BUFFER_SIZE]);
    void clearScreen();
    void exitDraw();
    void getMaxSize(I16 &w, I16 &h);

   private:
    DrawableUtils();

   private:
    Drawable *_drawable;
};

#endif