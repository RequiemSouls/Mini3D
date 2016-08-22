#include "DrawableUtils.h"

DrawableUtils &DrawableUtils::getInstance()
{
  static DrawableUtils instance;
  return instance;
}

#ifndef _WIN32
DrawableUtils::DrawableUtils()
{
  _drawable = new Drawable();
}

void DrawableUtils::buffer2Screen(
    I16 w, I16 h, Color buffer[BUFFER_SIZE][BUFFER_SIZE])
{
  _drawable->buffer2Screen(w, h, buffer);
}

void DrawableUtils::clearScreen()
{
  _drawable->clearScreen();
}

void DrawableUtils::exitDraw()
{
  _drawable->exitDraw();
}

void DrawableUtils::getMaxSize(I16 &w, I16 &h)
{
  _drawable->getMaxSize(w, h);
}

#else

DrawableUtils::DrawableUtils()
{
}

void DrawableUtils::buffer2Screen(
    I16 w, I16 h, Color buffer[BUFFER_SIZE][BUFFER_SIZE])
{
}

void DrawableUtils::clearScreen()
{
}

void DrawableUtils::exitDraw()
{
}

void DrawableUtils::getMaxSize(I16 &w, I16 &h)
{
}
#endif