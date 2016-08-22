#ifndef _WIN32

#include "Device.h"

#include <thread>
#include <chrono>
#include <time.h>
#include <vector>

Device::Device() {
}

void Device::drawPixel(I16 x, I16 y, Color color) {
	printf("I'm drawing pixel at: %d,%d\n", x, y);
}

void Device::drawLine(Vec2 from, Vec2 to, Color color) {

}

void Device::setLoopEvent(LoopEvent le) {
	loopEvent = le;
}


I8 Device::loop() {
	const I8 FRAME_TIME = 16;

	clock_t curDT = 0;
	clock_t mpfDT = FRAME_TIME;
	while (true) {
		curDT = clock();
		float FPS = 1000.0f / (int)mpfDT;
		if (FPS > 60.0f) FPS = 60.0f;

		printf("FPS: %.1f/%.1f\n", FPS, mpfDT / 1000.0);

		if (loopEvent != nullptr) {
			loopEvent();
		}

		mpfDT = clock() - curDT;

		if (mpfDT < FRAME_TIME) {
			this_thread::sleep_for(chrono::milliseconds(FRAME_TIME - mpfDT));
		}
	}
	return 0;
}

#endif