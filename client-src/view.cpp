#include "view.hpp"

void View::draw_imgui() {
    ImGui::ShowDemoWindow();

    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    {
        ImGui::Begin("Chat client", NULL, window_flags);
        ImGui::End();
    }
}
