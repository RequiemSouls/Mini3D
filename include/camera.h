#ifndef _MINI3D_CAMERA_H_
#define _MINI3D_CAMERA_H_

#include "geometry.h"
#include "types.h"
#include "config.h"

MINI_NS_BEGIN
//面向z轴的摄像机
class Camera {
public:
    Matrix GetMatrix() const;

    void set_near(F32 near) {near_ = near;}
    void set_ratio(const F32& ratio);
    const F32& ratio() const;

    void set_lookat(const Vector& lookat);
    const Vector& lookat() const;

    void SetCameraMode(bool isOrthogonal);
private:
    Matrix GetPerspective() const;
    Matrix GetOrthogonal() const;
    Matrix GetView() const;

private:
    F32 near_ = 200.0f;
    F32 far_ = 1000.0f;
    Vector lookat_ = Vector{0.0f, 0.0f, 1.0f, 1.0f};
    Vector pos_ = Vector{0.0f, 0.0f, 0.0f, 1.0f};
    /* /\*!< 看向一个物体时 摄像机的up向量与看向的物体和世界up向量的夹角 *\/ */
    /* F32 rotate_; */
    /*!< 水平方向上的张角 弧度制 90°*/
    F32 angle_ = PI * 0.5f;
    /*!< h/w */
    F32 ratio_ = 1.0f;
    bool isOrthogonal = false;
};

MINI_NS_END
#endif
