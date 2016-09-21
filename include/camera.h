#ifndef _MINI3D_CAMERA_H_
#define _MINI3D_CAMERA_H_

#include "geometry.h"
#include "types.h"

namespace mini3d {
//面向z轴的摄像机
class Camera {
public:
    Matrix GetMatrix() const;

private:
    F32 near_ = 500.0f;
    F32 far_ = 1000.0f;
    Vector lookup_{0.0f, 0.0f, 1.0f, 0.0f};
    Vector pos_{0.0f, 0.0f, 0.0f, 1.0f};
    /*!< 水平方向上的张角 弧度制 90°*/
    F32 angle_ = PI * 0.5;
    /*!< h/w */
    F32 ratio_ = 1.0f;
};

}  // namespace mini3d

#endif
