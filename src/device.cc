#include "device.h"
#include <stdio.h>
#include <SDL.h>
#include <thread>
#include <chrono>

#define FRAME_TIME 33
#define HEAD_SIZE 54

MINI_NS_BEGIN

SDL_Renderer* render;
Device &Device::GetInstance() {
    static Device instance;
    return instance;
}

Device::Device() {
    Init();
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

    I32 size = height_ * width_ * 3 + HEAD_SIZE;
    screen_buffer_ = (I8 *)malloc(size);
    memset(screen_buffer_, 0, size);

    I8* p = screen_buffer_;
    // 2 bytes bfType
    I16 type = 0x4d42;
    memcpy(p, &type, sizeof(I16));
    p += 2;
    // 4 bytes size
    memcpy(p, &size, sizeof(I32));
    p += 4;
    // 2 bytes 0
    p += 2;
    // 2 bytes 0
    p += 2;
    // 4 bytes data offset, default is 54
    I32 offset = 54;
    memcpy(p, &offset, sizeof(I32));
    p += 4;
    // head size default 40
    I32 headSize = 40;
    memcpy(p, &headSize, sizeof(I32));
    p += 4;
    // width height
    memcpy(p, &width_, sizeof(I16));
    memset(p + 2, 0, 2);
    p += 4;
    memcpy(p, &height_, sizeof(I16));
    memset(p + 2, 0, 2);
    p += 4;
    // planes default 1
    I16 plane = 1;
    memcpy(p, &plane, sizeof(I16));
    p += 2;
    // bitCount default 24
    I16 bitCount = 24;
    memcpy(p, &bitCount, sizeof(I16));
    p += 2;
    // compression default 0
    // size default 0
    memset(p, 0, sizeof(I32) * 6);

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    // SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    // SDL_RenderClear(render);

    return 0;
}

void Device::SetLoopEvent(LoopEvent &&le) { loop_event_ = std::move(le); }
void Device::SetKeyDownEvent(KeyDownEvent &&le) { keydown_event_ = std::move(le); }

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
        if (event.type == SDL_KEYDOWN) {
            keydown_event_(event.key.keysym.sym);
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
    I8* p = screen_buffer_ + 54;
    for (I16 y = 0; y < height_; ++y) {
        for (I16 x = 0; x < width_; ++x) {
            Color &rgb = buffer[x][y];
            if (rgb.r != 0) {
                count++;
            }

            memset(p, I32(rgb.b), 1);
            memset(p + 1, I32(rgb.g), 1);
            memset(p + 2, I32(rgb.r), 1);
            p += 3;
        }
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
                 "Mesh Count: %d FPS: %.1f/%.1fms point : %d %s\n",
                 mesh_count_, fps_, render_time_, count, log_);

    I32 size = height_ * width_ * 3 + HEAD_SIZE;
    SDL_RWops* ops = SDL_RWFromMem(screen_buffer_, size);
    SDL_Surface* su = SDL_LoadBMP_RW(ops, 1);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(render, su);
    SDL_FreeSurface(su);
    SDL_RenderCopy(render, tex, NULL, NULL);
    SDL_RenderPresent(render);
    SDL_DestroyTexture(tex);
}
MINI_NS_END
