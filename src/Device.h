#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <functional>

#include "geometry.h"
#include "ncurses.h"

#define BUFFER_SIZE 2048

class Device {
public:
    typedef std::function<void()> LoopEvent;

	static Device &getInstance();
    Device();

    void drawPixel(I16 x, I16 y, Color color);
    void drawLine(Vec2 from, Vec2 to, Color color);

    void setLoopEvent(LoopEvent &&le);
    I8 loop();

    void buffer2Screen(Color buffer[BUFFER_SIZE][BUFFER_SIZE]);
    void exitDraw();
    void getMaxSize(I16 &w, I16 &h);
    
    
private:
    I8 init();
    void init256ColorTable();

    LoopEvent loopEvent_;
    UI8 colorHash_[256] = {0};
    WINDOW *win_ = nullptr;
    I16 width_ = 0;
    I16 height_ = 0;
};

#endif