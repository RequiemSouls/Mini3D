#ifndef _MINI3D_CAMERA_H_
#define _MINI3D_CAMERA_H_

#include "geometry.h"
#include "types.h"

namespace mini3d {

//面向z轴的摄像机
class Camera{
public:
    Camera();
    ~Camera();
    Matrix GetMatrix();

private:
    F32 near_;
    F32 far_;
    Vector lookup_;
    Vector pos_;
    /*!< 水平方向上的张角 */
    F32 angle_;
    /*!< h/w */
    F32 ratio_;
};

}

#endif
