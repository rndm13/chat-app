#pragma once

#include "SDL.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengles2.h"
#include "SDL_events.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <stdio.h>

#include "../commmon-src/common_lib.hpp"

class View {
    SDL_GLContext gl_context;
    SDL_Window *window;

    void setup();
    void cleanup();

    i32 mouse_x = 0;
    i32 mouse_y = 0;

    bool done = false;
    void close();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void draw_imgui();

  public:
    View() { setup(); }

    ~View() { cleanup(); }

    void loop();

  private:
};
