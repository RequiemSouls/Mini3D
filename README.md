# 简介
做一个简单的软件渲染器，用来学习简单(但不仅限于简单)的图形学知识，好歹也是个游戏开发，图形学都不会就有点尴尬了，不是么，朋友~~
# 需求
## 基本
* 绘图API调用(support Linux/Mac/Windows/等 排名分先后)
* `void drawPoint(x, y, color)`
* `vector(x, y, z, w)`
* `matrix(float[16])`

## Vertex
* color
* vector

## Mesh
* `indexArray` 索引数组
* `vertexArray` 顶点数组
* `static Mesh genTrangle()` 创建一个三角形
* `static Mesh genByFile(fileName)` 读取文件创建mesh(i.e. *.obj)
* `void draw(render, matrix)` 画mesh到屏幕

## Camera
* `position` 位置
* `lookat` 方向
* `farPlane` 远截面
* `nearPlane` 近截面
* `viewAngle` 张角
* `ratio` 长宽比

## Render
* `renderbuffer` 渲染缓冲区
* `camera` 摄像机
* `meshArray` 准备渲染的mesh数组
* `void render()` 遍历mesh数组, 一个个渲染
* `void drawTrangle(p1, p2, p3)` 渲染一个三角形, 使用全局坐标
* `void buffer2Screen()` 把渲染缓冲区使用平台的画图api画到屏幕上

## drawTrangle()
* cull 挑选/剔除, 完全在视锥体之外的丢掉不要, 完全在视锥体之内的物体不需要裁剪
* cut 以视锥体裁剪出可见区域
* perspective 透视出近大远小的效果
* projection 投影到near面上
* rasterization 根据分割的图元光栅出点/像素
* interpolation 差值出点的颜色