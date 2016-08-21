#pragma once

#include "geometry.h"
#include <functional>

using namespace std;

struct Device {
	typedef function<void()> LoopEvent;

	Device();

	void drawPixel(int x, int y, Color color);
	void drawLine(Vec2 from, Vec2 to, Color color);

	void setLoopEvent(LoopEvent le);
	int loop();

	LoopEvent loopEvent = nullptr;
};