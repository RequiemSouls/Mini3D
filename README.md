# Introduction
A simple software renderer

# Install
install SDL2 & CMake.
## Mac
`brew install cmake`
`brew install sdl2`
`cd Mini3D`
`mkdir build`
`cd build`
`cmake ..`
`make`
## Windows
## linux

# Code Sytle
* [Google C++ code naming](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/)
* [Google C++ code formatting](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/formatting/)

# Documents
## Geomtry
use google-test to ensure correntness 
* `vector(x, y, z, w)`
* `color(r, g, b)`
* `matrix(F32[4][4])`

## Device
device&platform initalize (use sdl2)
* `void Buffer2Screen(Color **buffer)` show `RenderBuffer` on screen
* `void SetLoopEvent(LoopEvent &&le)` program main loop
* `void SetKeyDownEvent(KeyDownEvent &&kde)` program keyboward handle event

## Vertex
* Now a vertex includes the `color` & `position(vector)` and will include `UV(vector2)` later on.

## Mesh
Mesh is used for read model file and save model data(indexArray & vertexArray)
* `static Mesh genTriangle()` generate a random triangle
* `static Mesh genByFile(fileName)` load data from model file(currently only supported *.obj file)
* `void draw(render, matrix)` draw the mesh on screen

## Camera
* `viewAngle` The angle between left and right panel
* `farPlane, nearPlane` near, far, left, right panel surrounded the visible area
* `position` camera world space position
* `lookat(vector)` camera look direction
* `ratio` viewport height / width

## Render
* `render_buffer` render buffer used for storage a frame color data. the same size as the screen width * height
* `deep_buffer` deep buffer used for storage a frame deep data, is also called z-buffer.
* `camera` a camera instance
* `device` a device instance
* `void DrawTriangle(vt1, vt2, vt3, m)` draw a triangle. 3D position projection to 2D panel use camera projection matrix

## drawTrangle()
* `cull` discard the triangle outside the view frustum
* `rasterization` computer fragment depend on screen position `interpolation`

# TODO
## Render
* `cut`
* `texture`
* `material`
* `light`

## Tool
* Introducing `imgui` to improve usability
