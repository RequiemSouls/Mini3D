#ifdef _WIN32

#include "Device.h"

#include <vector>
#include <thread>
#include <chrono>
#include <time.h>

#include <Windows.h>
#include <WindowsX.h>

namespace DeviceWin32 {
	struct Windows* g_windowInstance = nullptr;
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	struct Windows {
		const int FRAME_TIME = 16;

		UI32 bgColor = 0x0;

		int width = 960;
		int height = 640;

		bool isQuit = false;

		HWND hwnd = nullptr;
		HDC hDC = nullptr;
		HBITMAP foreBM;
		HBITMAP backBM;
		UI32** frameBuff;
		MSG curMSG;

		Device::LoopEvent loopEvent;

		static Windows* getInstance() {
			static Windows windows;
			g_windowInstance = &windows;
			return &windows;
		}

		Windows() {
			initWindow(width, height);
		}

		Windows(int width, int height) {
			initWindow(width, height);
		}

		HWND getHWnd() {
			return hwnd;
		}

		bool initWindow(int width, int height) {
			destroy();

			WNDCLASS wc = {
				CS_BYTEALIGNCLIENT,
				(WNDPROC)WindowProc, 0, 0, 0,
				nullptr, nullptr, nullptr, nullptr, "MINI3D" 
			};

			wc.hInstance = GetModuleHandle(NULL);
			if (!RegisterClass(&wc)) return false;

			hwnd = CreateWindowA(
				"MINI3D", "MINI3D",
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				0, 0, 0, 0, nullptr, nullptr,
				wc.hInstance, nullptr);
			if (hwnd == nullptr)
				return false;

			HDC dc = GetDC(hwnd);
			hDC = CreateCompatibleDC(dc);
			ReleaseDC(hwnd, dc);

			LPVOID ptr;
			BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, width * height * 4, 0, 0, 0, 0 } };
			foreBM = CreateDIBSection(hDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
			backBM = (HBITMAP)SelectObject(hDC, foreBM);
			UI8* screenBuff = (UI8*)ptr;

			int cx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
			int cy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
			SetWindowPos(hwnd, nullptr, cx, cy, width, height, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
			SetForegroundWindow(hwnd);
			ShowWindow(hwnd, SW_NORMAL);

			memset(screenBuff, 0, width * height * 4);

			frameBuff = new UI32*[height];
			for (int h = 0; h < height; h++) {
				frameBuff[h] = (UI32*)(screenBuff + width * 4 * h);
			}

			return true;
		}

		LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
				case WM_LBUTTONDOWN: {
					int x = GET_X_LPARAM(lParam);
					int y = GET_Y_LPARAM(lParam);
					break;
				}
				case WM_LBUTTONUP: {
					int x = GET_X_LPARAM(lParam);
					int y = GET_Y_LPARAM(lParam);
					break;
				}
				case WM_RBUTTONDOWN: {
					int x = GET_X_LPARAM(lParam);
					int y = GET_Y_LPARAM(lParam);
					break;
				}
				case WM_RBUTTONUP: {
					int x = GET_X_LPARAM(lParam);
					int y = GET_Y_LPARAM(lParam);
					break;
				}
				case WM_MOUSEMOVE: {
					int x = GET_X_LPARAM(lParam);
					int y = GET_Y_LPARAM(lParam);
					break;
				}
				//case WM_PAINT: {
				//	break;
				//}
				case WM_DESTROY: {
					PostQuitMessage(0);
					break;
				}
				case WM_CLOSE: {
					isQuit = true; 
					break;
				}
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}

		void destroy (){
			if (hDC) {
				if (backBM) {
					SelectObject(hDC, backBM);
					backBM = nullptr;
				}
				DeleteDC(hDC);
				backBM = nullptr;
				hDC = nullptr;
			}
			if (foreBM) {
				DeleteObject(foreBM);
				foreBM = nullptr;
			}
			if (hwnd) {
				CloseWindow(hwnd);
				hwnd = nullptr;
			}
			if (frameBuff) {
				delete[] frameBuff;
				frameBuff = nullptr;
			}
		}

		void beginDraw() {
			clearScreen();
		}

		void endDraw() {
			HDC dc = GetDC(hwnd);
			BitBlt(dc, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
			ReleaseDC(hwnd, dc);
		}

		int loop() {
			clock_t curDT = 0;
			clock_t mpfDT = FRAME_TIME;
			int countFrame = 0;
			while (true)
			{
				if (isQuit) break;
				if (!PeekMessage(&curMSG, nullptr, 0, 0, PM_REMOVE)){
					curDT = clock();
					float FPS = 1000.0f / (int)mpfDT;
					if (FPS > 60.0f) FPS = 60.0f;

					printf("FPS: %.1f/%.1f  F:%d\n", FPS, mpfDT / 1000.0, countFrame++);

					beginDraw();
					if (loopEvent != nullptr) {
						loopEvent();
					}
					endDraw();

					mpfDT = clock() - curDT;

					if (mpfDT < FRAME_TIME) {
						this_thread::sleep_for(chrono::milliseconds(FRAME_TIME - mpfDT));
					}
				}

				TranslateMessage(&curMSG);
				DispatchMessage(&curMSG);
			}
			return 0;
		}

		void drawPixel(int x, int y, Color color) {
			frameBuff[y][x] = color.r << 16 | color.g << 8 | color.b;
		}

		void clearScreen() {
			for (int y = 0; y < height; y++) {
				UI32 *dst = frameBuff[y];
				for (int x = width; x > 0; dst++, x--) dst[0] = bgColor;
			}
		}
	};

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (g_windowInstance && g_windowInstance->getHWnd() == hwnd) {
			g_windowInstance->windowProc(hwnd, msg, wParam, lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}


Device::Device() {
	DeviceWin32::Windows::getInstance();
}

void Device::drawPixel(int x, int y, Color color)
{
	DeviceWin32::g_windowInstance->drawPixel(x, y, color);
}

void Device::drawLine(Vec2 from, Vec2 to, Color color)
{
	Vec2 v = to - from;
	float len = v.length();
	v.normalized();
	while (len > 0) {
		DeviceWin32::g_windowInstance->drawPixel(from.x, from.y, color);
		from += v;
		len -= 1;
	}
}

void Device::setLoopEvent(LoopEvent le)
{
	loopEvent = le;
	DeviceWin32::g_windowInstance->loopEvent = le;
}


int Device::loop() {
	int ret = DeviceWin32::g_windowInstance->loop();
	return ret;
}

#endif