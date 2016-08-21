#include "DrawableUtils.h"

DrawableUtils& DrawableUtils::getInstance() {
    static DrawableUtils instance;
    return instance;
}


#ifndef _WIN32
DrawableUtils::DrawableUtils() {
    _drawable = new Drawable();
}

void DrawableUtils::buffer2Screen(int w, int h, Color buffer[2048][2048]) {
    _drawable->buffer2Screen(w, h, buffer);
}

void DrawableUtils::clearScreen() {
    _drawable->clearScreen();
}

void DrawableUtils::exitDraw() {
    _drawable->exitDraw();
}

void DrawableUtils::getMaxSize(int& w, int& h) {
    _drawable->getMaxSize(w, h);
}

#else

DrawableUtils::DrawableUtils() {

}

void DrawableUtils::buffer2Screen(int w, int h, Color buffer[2048][2048]) {

}

void DrawableUtils::clearScreen() {

}

void DrawableUtils::exitDraw() {

}

void DrawableUtils::getMaxSize(int& w, int& h) {

}
#endif