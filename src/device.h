#ifndef MINI3D_DEVICE_H_
#define MINI3D_DEVICE_H_

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
    void GetMaxSize(I16 *w, I16 *h) { *w = width_; *h = height_; }
    void set_mesh_count(I32 count) { mesh_count_ = count; }
    void set_log(I8 *log) { strncpy(log_, log, sizeof(log_)); }
    void set_log(I32 num);
    
private:
    I8 Init();
    void Init256ColorTable();

    LoopEvent loop_event_ = nullptr;
    UI8 color_hash_[256] = {0};
    I16 width_ = 0;
    I16 height_ = 0;
    I32 mesh_count_ = 0;
    F32 fps_ = 0.0;
    F32 render_time_ = 0.0;
    I32 frame_count_ = 0;
    I8 log_[128] = {0};
    Color **screen_buffer_ = nullptr;
};

}  // namespace mini3d

#endif  // MINI3D_DEVICE_H_
