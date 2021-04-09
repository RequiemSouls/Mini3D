#include "display.h"
#include "renderer.h"
#include "ui/imgui.h"

#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#endif
void draw_ui() {
}

int main(int /*argc*/, char ** /*args*/) {
#ifndef _WIN32
  srand(time(0));
#else
  SYSTEMTIME st;
  GetSystemTime(&st);
  srand(st.wMilliseconds);
#endif

  // init render engin
  mini3d::Device &device = mini3d::Device::GetInstance();
  mini3d::Renderer *r = new mini3d::Renderer(device);
  // mini3d::Display *root =
  //  new mini3d::Display("../asset/avg_captainamerica_0.obj");
  // create root display
  mini3d::Display *root = new mini3d::Display("../asset/cube.obj");
  // mini3d::Display *root = new mini3d::Display("../asset/triangle.obj");
  mini3d::Vector pos{0, 0, 500, 1};
  mini3d::Vector scale{1, 1, 1, 0};
  mini3d::Vector rotate{0, 1, 0, 0};
  mini3d::F32 delta = 0;
  mini3d::F32 deltaOffset = 0;
  bool stop = false;
  mini3d::Camera &camera = r->camera();

  device.SetKeyDownEvent([&stop, &deltaOffset, &rotate](mini3d::I32 type) {
    switch (type) {
    case SDLK_q:
      stop = true;
      break;
    case SDLK_d:
      stop = false;
      rotate.set_x(0);
      rotate.set_y(1);
      rotate.set_z(0);
      deltaOffset = -0.03;
      break;
    case SDLK_a:
      rotate.set_x(0);
      rotate.set_y(1);
      rotate.set_z(0);
      deltaOffset = 0.03;
      stop = false;
      break;
    case SDLK_s:
      rotate.set_x(1);
      rotate.set_y(0);
      rotate.set_z(0);
      deltaOffset = -0.03;
      stop = false;
      break;
    case SDLK_w:
      rotate.set_x(1);
      rotate.set_y(0);
      rotate.set_z(0);
      deltaOffset = 0.03;
      stop = false;
      break;
    }
  });
  // init ui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  short w, h;
  device.GetMaxSize(&w, &h);
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(960, 640);
  unsigned char* pixels;
  int width, height, bytes_per_pixel;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
  int x, y;
  bool leftDown, rightDown, midDown;
  device.SetInputEvent([&io, &x, &y, &leftDown, &rightDown, &midDown](mini3d::InputEventData event) {
    switch (event.inputType) {
        case mini3d::InputType::MouseWheel: {
            if (event.x < 0) io.MouseWheelH += 1;
            if (event.x > 0) io.MouseWheelH -= 1;
            if (event.y < 0) io.MouseWheel += 1;
            if (event.y > 0) io.MouseWheel -= 1;
            break;
        }
        case mini3d::InputType::KeyRelease: 
        case mini3d::InputType::KeyDown: {
            io.KeysDown[event.value] = event.inputType == mini3d::InputType::KeyDown;
            break;
        }
        case mini3d::InputType::MouseDown: {
            leftDown = event.keyType == mini3d::KeyType::MouseLeft;
            rightDown = event.keyType == mini3d::KeyType::MouseRight;
            midDown = event.keyType == mini3d::KeyType::MouseMid;
            x = event.x;
            y = event.y;
            break;
        }
        default: {
            break;
        }
    }
  });


  bool show_demo_window = true;
  device.SetLoopEvent([&io, &stop, &deltaOffset, &rotate, &delta, &scale, &camera,
                       &pos, &r, &root, &show_demo_window, &x, &y, &leftDown, &rightDown, &midDown](float dt) {
    if (stop) {
      return;
    }
    ImGui::NewFrame();
    io.MouseDown[0] = leftDown;
    io.MouseDown[1] = rightDown;
    io.MouseDown[2] = midDown;
    leftDown = rightDown = midDown = false;
    io.MousePos = ImVec2((float)x, (float)y);
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    // const ImGuiViewport* viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(true ? viewport->WorkPos : viewport->Pos);
    // ImGui::SetNextWindowSize(true ? viewport->WorkSize : viewport->Size);
    // ImGui::Begin("2");
    // ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();

    // transfer test
    // pos.set_z(((int)pos.get_z() + 1) % 500 + 500);
    root->set_pos(pos);
    // camera lookat test
    camera.set_lookat(pos);
    // rotate test
    delta += deltaOffset;
    root->set_rotate_v(rotate);
    root->set_rotate_d(delta);
    // scale test
    root->set_scale(scale);
    r->Clean();
    // draw ui
    ImDrawData* draw_data = ImGui::GetDrawData();
    mini3d::Mesh uiMesh = mini3d::Mesh::GenByImDrawData(draw_data);
    camera.SetCameraMode(true);
    uiMesh.Draw(r, mini3d::Matrix::IDENTITY());
    camera.SetCameraMode(false);
    root->Draw(r);
    r->Buffer2Screen();
  });

  mini3d::I32 ret = device.Loop();
  return ret;
}