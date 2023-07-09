#include "view.hpp"

void View::draw_imgui() {
    ImGui::ShowDemoWindow();

    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    {
        ImGui::Begin("Chat client", NULL, window_flags);
        if (ImGui::Button("Sleep for 1 second")) {
            controller->do_member(&Controller::wait, boost::asio::chrono::seconds(1));
        }

        ImGui::End();
    }
}
