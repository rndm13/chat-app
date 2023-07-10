#include "controller.hpp"
#include "view.hpp"
#include <boost/system/system_error.hpp>
#include <string_view>

void View::draw_imgui() {
    ImGui::ShowDemoWindow();

    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    {
        ImGui::Begin("Chat client", NULL, window_flags);

        ImGui::InputText("Host name: ", host_name.data(), host_name.size());
        if (ImGui::Button("Connect")) {
            controller->do_member(
                &Controller::connect_to,
                std::string_view(host_name.data(), host_name.size()));
        }

        std::string str_messages =
            fmt::format("{}", fmt::join(model.messages, "\n"));

        ImGui::InputTextMultiline("", str_messages.data(), str_messages.size());

        ImGui::InputText("Username: ", user_name.data(), user_name.size());
        ImGui::InputText("Message text: ", message_contents.data(),
                         message_contents.size());

        if (ImGui::Button("Send")) {
            controller->do_member(&Controller::send_message,
                                  Message(message_contents, user_name));
        }

        ImGui::End();
    }
}
