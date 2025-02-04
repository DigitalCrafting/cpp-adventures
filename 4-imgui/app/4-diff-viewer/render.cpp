#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
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

        DrawSelection();
        DrawDiffView();
        DrawStats();

        ImGui::End();
    }

    void WindowClass::DrawSelection() {
        ImGui::InputText("Left", &filePath1);
        ImGui::SameLine();
        if (ImGui::Button("Save###Left"))
            SaveFileContent(filePath1, fileContent1);

        ImGui::InputText("Right", &filePath2);
        ImGui::SameLine();
        if (ImGui::Button("Save###Right"))
            SaveFileContent(filePath2, fileContent2);

        if (ImGui::Button("Compare")) {
            fileContent1 = LoadFileContent(filePath1);
            fileContent2 = LoadFileContent(filePath2);

            CreateDiff();
        }
    }

    void WindowClass::DrawDiffView() {

    }

    void WindowClass::DrawStats() {

    }

    WindowClass::FileContent WindowClass::LoadFileContent(std::string_view file_path) {
        return FileContent({});
    }

    void WindowClass::SaveFileContent(std::string_view file_path,
                                      ImGuiDiffViewer::WindowClass::FileContent &fileContent) {

    }

    void WindowClass::CreateDiff() {

    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Diff Viewer");
    }
} // namespace