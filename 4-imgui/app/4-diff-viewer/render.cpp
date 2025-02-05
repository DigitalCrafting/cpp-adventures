#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <fmt/format.h>

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
        constexpr static auto swap_width = 40.0F;
        const auto parent_size = ImVec2(ImGui::GetContentRegionAvail().x, 500.0F);
        const auto child_size = ImVec2(parent_size.x / 2.0F - swap_width, parent_size.y);
        const auto swap_size = ImVec2(swap_width, child_size.y);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
        ImGui::BeginChild("Parent", parent_size, true);

        if (ImGui::BeginChild("Diff1", child_size, false)) {
            for (std::size_t i = 0; i < fileContent1.size(); ++i) {
                if (!diffResult1[i].empty()) {
                    ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F), "%s", fileContent1[i].data());
                } else {
                    ImGui::Text("%s", fileContent1[i].data());
                }
            }
        }
        ImGui::EndChild(); // Diff1
        ImGui::SameLine();

        const auto line_height = ImGui::GetTextLineHeightWithSpacing();
        const auto button_size = ImVec2(15.0F, line_height);

        if (ImGui::BeginChild("Swap", swap_size, true)) {
            for (std::size_t i = 0; i < diffResult1.size(); ++i) {
                const auto left_label = fmt::format("<###{}", i);
                const auto right_label = fmt::format(">###{}", i);

                if (!diffResult1.empty() || !diffResult2.empty()) {
                    if (ImGui::Button(left_label.data(), button_size)) {
                        if (fileContent1.size() > i && fileContent2.size() > i) {
                            fileContent1[i] = fileContent2[i];
                        } else if(fileContent2.size() > i) {
                            fileContent1.push_back(fileContent2[i]);
                        }
                        CreateDiff();
                    }

                    ImGui::SameLine();

                    if (ImGui::Button(right_label.data(), button_size)) {
                        if (fileContent1.size() > i && fileContent2.size() > i) {
                            fileContent2[i] = fileContent1[i];
                        } else if(fileContent1.size() > i) {
                            fileContent2.push_back(fileContent1[i]);
                        }
                        CreateDiff();
                    }
                } else {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + line_height);
                }
            }
        }
        ImGui::EndChild(); // Swap
        ImGui::SameLine();

        if (ImGui::BeginChild("Diff2", child_size, false)) {
            for (std::size_t i = 0; i < fileContent2.size(); ++i) {
                if (!diffResult2[i].empty()) {
                    ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F), "%s", fileContent2[i].data());
                } else {
                    ImGui::Text("%s", fileContent2[i].data());
                }
            }
        }
        ImGui::EndChild(); // Diff2
        ImGui::EndChild(); // Parent
        ImGui::PopStyleVar();
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