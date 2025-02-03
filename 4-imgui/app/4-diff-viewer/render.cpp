#include <imgui.h>
#include <valarray>

#include "render.hpp"

namespace ImGuiDiffViewer {
    void WindowClass::Draw(std::string_view label) {
        constexpr static auto window_flags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
        constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);

        ImGui::Begin(label.data(), nullptr, window_flags);

        ImGui::End();
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Diff Viewer");
    }
} // namespace