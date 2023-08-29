#pragma once

#include "SDL.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengles2.h"
#include "SDL_events.h"

#include "controller.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/system/system_error.hpp>
#include <stdio.h>
#include <thread>

#include "../common-src/common_lib.hpp"

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
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

    void draw_imgui();

  public:
    View(boost::asio::thread_pool &_thr_pool)
        : controller(Controller::create(_thr_pool)),
          model(controller->get_model()) {
        setup();
    }

    ~View() { cleanup(); }

    void loop();

  private:
    Controller::pointer controller;
    const Model &model;

    boost::array<char, 128> host_name{};
    boost::array<char, 256> message_contents{};
    boost::array<char, 64> user_name{};
};
