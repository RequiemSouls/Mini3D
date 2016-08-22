#pragma once

#include <functional>

#include "geometry.h"

struct Device {
  typedef std::function<void()> LoopEvent;

  Device();

  void drawPixel(I16 x, I16 y, Color color);
  void drawLine(Vec2 from, Vec2 to, Color color);

  void setLoopEvent(LoopEvent le);
  I8 loop();

  LoopEvent loopEvent = nullptr;
};
