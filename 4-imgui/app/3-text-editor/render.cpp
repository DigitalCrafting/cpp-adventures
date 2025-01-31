#include <imgui.h>
#include <valarray>
#include <implot.h>
#include <filesystem>

#include "render.hpp"
#include "fmt/format.h"

namespace fs = std::filesystem;

namespace ImGuiTextEditor {
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

    void WindowClass::DrawMenu() {}

    void WindowClass::DrawContent() {}

    void WindowClass::DrawInfo() {}

    void WindowClass::DrawLoadPopup() {}

    void WindowClass::DrawSavePopup() {}

    void WindowClass::SaveToFile(std::string_view fileName) {}

    void WindowClass::LoadToFile(std::string_view fileName) {}

    std::string WindowClass::GetFileExtension(std::string_view fileName) {}

    void render(WindowClass &window_obj) {
        window_obj.Draw("Text editor");
    }
} // namespace