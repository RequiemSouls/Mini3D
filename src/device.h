#ifndef _MINI3D_DEVICE_H_
#define _MINI3D_DEVICE_H_

#include <functional>

#include "geometry.h"
#include "ncurses.h"

namespace mini3d {

class Device {
public:
    typedef std::function<void()> LoopEvent;

	static Device &GetInstance();
    Device();
    ~Device();
    
    I8 Loop();
    void SetLoopEvent(LoopEvent &&le);
    void Buffer2Screen(Color **buffer);
    void ExitDraw();
    void GetMaxSize(I16 &w, I16 &h);
    void set_mesh_count(I32 count) {mesh_count_ = count; }

private:
    I8 Init();
    void Init256ColorTable();

    LoopEvent loop_event_;
    UI8 color_hash_[256] = {0};
    I16 width_ = 0;
    I16 height_ = 0;
    I16 bottom_offset_ = 1;
    I32 mesh_count_ = 0;
    Color **screen_buffer_;
};

}  // namespace mini3d

#endif
