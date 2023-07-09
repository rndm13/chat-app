#include "view.hpp"
void View::wait() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    fmt::print("{} Waited for a whole second!\n", std::this_thread::get_id());
}

void View::draw_imgui() {
    ImGui::ShowDemoWindow();

    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    {
        ImGui::Begin("Chat client", NULL, window_flags);
        if (ImGui::Button("Sleep for 1 second")) {
            boost::asio::dispatch(thr_pool, boost::bind(&View::wait, shared_from_this()));
        }
        ImGui::End();
    }
}
