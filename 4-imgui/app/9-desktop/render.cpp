#include <imgui.h>

#include "render.hpp"
#include "fmt/format.h"

namespace ImGuiDesktop {
    void WindowClass::Draw(std::string_view label) {
        constexpr static auto window_flags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs;
        constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
        constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);

        ImGui::Begin(label.data(), nullptr, window_flags);

        DrawDesktop();
        DrawTaskbar();

        ImGui::End();
    }

    void WindowClass::DrawDesktop() {
        for (auto &icon: icons) {
            icon.Draw();
        }
    }

    void WindowClass::DrawTaskbar() {
        constexpr static auto taskbar_flags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoTitleBar;
        constexpr static auto taskbar_size = ImVec2(1280.0F, 40.0F);
        constexpr static auto taskbar_pos = ImVec2(0.0F, 680.0F);

        static auto open_taskbar = false;

        ImGui::SetNextWindowSize(taskbar_size);
        ImGui::SetNextWindowPos(taskbar_pos);

        ImGui::Begin("Taskbar", nullptr, taskbar_flags);

        if (ImGui::Button("All Icons", ImVec2(100, 30))) {
            ImGui::OpenPopup("My Programs");
            open_taskbar = true;
        }

        if (open_taskbar) {
            ShowIconList(&open_taskbar);
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x);

        static auto clock_open = false;
        clock.GetTime();
        const auto time = fmt::format("{}:{}", clock.hrs, clock.mins);
        if (ImGui::Button(time.data(), ImVec2(100.0F, 30.0F)) || clock_open) {
            clock.Draw("clockWindow");
            clock_open = true;
        }
        if (clock_open && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            clock_open = false;
        }
        ImGui::End();
    }

    void WindowClass::ShowIconList(bool *open) {}

    void WindowClass::Icon::Draw() {}

    void render(WindowClass &window_obj) {
        window_obj.Draw("Desktop");
    }
} // namespace