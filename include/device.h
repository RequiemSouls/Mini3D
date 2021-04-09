#ifndef MINI3D_DEVICE_H_
#define MINI3D_DEVICE_H_

#include <functional>

#include <string.h>
#include "geometry.h"
#include "config.h"

MINI_NS_BEGIN
enum InputType {
    None,
    KeyDown,
    KeyRelease,
    MouseDown,
    MouseWheel
};
enum KeyType {
    MouseLeft,
    MouseRight,
    MouseMid,
};
struct InputEventData {
    InputType inputType;
    KeyType keyType;
    I32 value;
    I32 x;
    I32 y;
    const char* stringValue;
};
class Device {
public:
    typedef std::function<void(F32)> LoopEvent;
    typedef std::function<void(I32)> KeyDownEvent;
    typedef std::function<void(InputEventData)> InputEvent;

    static Device &GetInstance();
    ~Device();

    I8 Loop();
    void SetLoopEvent(LoopEvent &&le);
    void SetKeyDownEvent(KeyDownEvent &&kde);
    void SetInputEvent(InputEvent &&ie);
    void Buffer2Screen(Color **buffer);
    void GetMaxSize(I16 *w, I16 *h) {
        *w = width_/sampleRate_;
        *h = height_/sampleRate_;
    }

    void set_mesh_count(I32 count) { mesh_count_ = count; }
    void set_log(I8 *log) { strncpy(log_, log, sizeof(log_)); }
    void set_log(I32 num);

private:
    // Private constructor ensures this object will not be able to create
    // directly, only create it by using static function GetInstance().
    Device();

    I8 Init();

    LoopEvent loop_event_ = nullptr;
    KeyDownEvent keydown_event_ = nullptr;
    InputEvent input_event_ = nullptr;
    UI8 color_hash_[256];
    I16 width_ = 0;
    I16 height_ = 0;
    I32 mesh_count_ = 0;
    F32 fps_ = 0.0;
    F32 render_time_ = 0.0;
    I8 log_[128];
    I8 *screen_buffer_ = nullptr;
    F32 sampleRate_ = 1;
};

MINI_NS_END
#endif  // MINI3D_DEVICE_H_
