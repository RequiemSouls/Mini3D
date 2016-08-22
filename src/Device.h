#pragma once

#include "geometry.h"
#include <functional>

using namespace std;

struct Device {
  typedef function<void()> LoopEvent;

  Device();

  void drawPixel(I16 x, I16 y, Color color);
  void drawLine(Vec2 from, Vec2 to, Color color);

  void setLoopEvent(LoopEvent le);
  I8 loop();

  LoopEvent loopEvent = nullptr;
};