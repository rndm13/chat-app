#pragma once

#include "SDL.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengles2.h"
#include "SDL_events.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <boost/asio/io_context.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/chrono/duration.hpp>
#include <stdio.h>
#include <thread>

#include "../commmon-src/common_lib.hpp"

class View : public boost::enable_shared_from_this<View> {

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
    View(boost::asio::thread_pool& _thr_pool) 
        : thr_pool(_thr_pool) { 
            setup();
        }

    ~View() { cleanup(); }

    void loop();

  private:
    void wait();
    boost::asio::thread_pool& thr_pool;

  public:

    typedef boost::shared_ptr<View> pointer;
    static pointer create(boost::asio::thread_pool &thr_pool) {
        return pointer(new View(thr_pool));
    }
};
