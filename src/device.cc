#include "device.h"
#include <stdio.h>
#include <SDL.h>
#include <thread>
#include <chrono>

#define FRAME_TIME 33
namespace mini3d {
    SDL_Renderer* render;
    Device &Device::GetInstance() {
        static Device instance;
        return instance;
    }

    Device::Device() {
        I8 succ = Init();
    }

    Device::~Device() {};

    I8 Device::Init() {
        width_ = 960;
        height_ = 640;
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Couldn't initialize SDL: %s", SDL_GetError());
            return 1;
        }

        SDL_Window *win;
        int succ = SDL_CreateWindowAndRenderer(
            width_, height_, SDL_WINDOW_MOUSE_FOCUS, &win, &render);
        if (succ) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Couldn't create window and renderer: %s", SDL_GetError());

            SDL_Quit();
            return 1;
        }
        const char* title = "mini3d~~";
        SDL_SetWindowTitle(win, title);

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

        screen_buffer_ = (Color **)malloc(sizeof(Color *) * width_);
        for (int i = 0; i < width_; ++i) {
            screen_buffer_[i] = (Color *)malloc(height_ * sizeof(Color));
            memset(screen_buffer_[i], 0xff, height_ * sizeof(Color));
        }
        return 0;
    }

    void Device::SetLoopEvent(LoopEvent &&le) { loop_event_ = std::move(le); }

    I8 Device::Loop() {
        I16 clocks_per_ms = CLOCKS_PER_SEC / 1000.0;
        const I32 frameTime = FRAME_TIME * clocks_per_ms;

        clock_t curDT = 0;
        clock_t mpfDT = frameTime;

        SDL_Event event;
        while (true) {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                break;
            }

            curDT = clock();
            fps_ = CLOCKS_PER_SEC / (1.0 * mpfDT);
            if (fps_ > 60.0f) fps_ = 60.0f;
            render_time_ = mpfDT / (1.0 * clocks_per_ms);

            if (loop_event_ != nullptr) {
                loop_event_();
            }
            mpfDT = clock() - curDT;

            if (mpfDT < frameTime) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds((frameTime - mpfDT) / clocks_per_ms));
            }
        }
        SDL_Quit();
        return 0;
    }

    void Device::Buffer2Screen(Color **buffer) {
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        int count = 0;
        for (I16 x = 0; x < width_; ++x) {
            for (I16 y = 0; y < height_; ++y) {
                Color &rgb = buffer[x][y];
                if (rgb.r != 0) {
                    count++;
                }

                if (screen_buffer_[x][y] != rgb) {
                    screen_buffer_[x][y] = rgb;
                }
                SDL_SetRenderDrawColor(render, rgb.r, rgb.g, rgb.b, 255);
                SDL_RenderDrawPoint(render, x, y);
            }
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
                 "Mesh Count: %d FPS: %.1f/%.1fms point : %d %s\n",
                 mesh_count_, fps_, render_time_, count, log_);
        SDL_RenderPresent(render);
    }
}
