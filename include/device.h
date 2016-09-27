#ifndef MINI3D_DEVICE_H_
#define MINI3D_DEVICE_H_

#include <functional>

#include <string.h>
#include "geometry.h"

namespace mini3d {

class Device {
public:
    typedef std::function<void()> LoopEvent;

    static Device &GetInstance();
    ~Device();

    I8 Loop();
    void SetLoopEvent(LoopEvent &&le);
    void Buffer2Screen(Color **buffer);
    void ExitDraw();
    void GetMaxSize(I16 *w, I16 *h) {
        *w = width_;
        *h = height_;
    }

    void set_mesh_count(I32 count) { mesh_count_ = count; }
    void set_log(I8 *log) { strncpy(log_, log, sizeof(log_)); }
    void set_log(I32 num);

private:
    // Private constructor ensures this object will not be able to create
    // directly, only create it by using static function GetInstance().
    Device();

    I8 Init();
    void Init256ColorTable();

    LoopEvent loop_event_ = nullptr;
    UI8 color_hash_[256];
    I16 width_ = 0;
    I16 height_ = 0;
    I32 mesh_count_ = 0;
    F32 fps_ = 0.0;
    F32 render_time_ = 0.0;
    I32 frame_count_ = 0;
    I8 log_[128];
    Color **screen_buffer_ = nullptr;
};

}  // namespace mini3d

#endif  // MINI3D_DEVICE_H_
