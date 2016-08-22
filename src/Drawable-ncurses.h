#ifndef _DRAWABLE_NCURSES_H_
#define _DRAWABLE_NCURSES_H_

#include "geometry.h"
#define BUFFER_SIZE 2048

class Drawable {
public:
  Drawable();
  void drawPoint(I32 index, I16 x, I16 y, Color rgb);
  void buffer2Screen(I16 w, I16 h, Color buffer[BUFFER_SIZE][BUFFER_SIZE]);
  void clearScreen();
  void exitDraw();
  void getMaxSize(I16 &w, I16 &h);

private:
  I8 init();
  UI8 get256ColorIndex(UI8 rgb);
  UI8 get256ColorRGB(I32 rgb);
  UI8 get256ColorRGB(Color rgb);
};

#endif