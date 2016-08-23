#ifndef _MINI3D_DEVICE_H_
#define _MINI3D_DEVICE_H_

#include <functional>

#include "geometry.h"
#include "ncurses.h"

#define BUFFER_SIZE 2048

class Device {
public:
    typedef std::function<void()> LoopEvent;

	static Device &GetInstance();
    Device();

    void SetLoopEvent(LoopEvent &&le);
    I8 Loop();
    void Buffer2Screen(Color buffer[BUFFER_SIZE][BUFFER_SIZE]);
    void ExitDraw();
    void GetMaxSize(I16 &w, I16 &h);
    
    
private:
    I8 Init();
    void Init256ColorTable();

    LoopEvent loopEvent_;
    UI8 colorHash_[256] = {0};
    WINDOW *win_ = nullptr;
    I16 width_ = 0;
    I16 height_ = 0;
};

#endif