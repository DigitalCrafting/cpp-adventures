#include <imgui.h>
#include <valarray>
#include <filesystem>
#include <fstream>

#include "render.hpp"

namespace fs = std::filesystem;

namespace ImGuiCsvEditor {
    static constexpr std::string_view SAVE_POPUP_NAME = "Save File";
    static constexpr std::string_view LOAD_POPUP_NAME = "Load File";

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

    void WindowClass::DrawSavePopup() {
        static char saveFilenameBuffer[256] = "text.txt";
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(SAVE_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", saveFilenameBuffer, sizeof(saveFilenameBuffer));

            if (ImGui::Button("Save", popUpButtonSize)) {
                SaveToCsvFile(saveFilenameBuffer);
//                currentFileName = saveFilenameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void WindowClass::DrawLoadPopup() {
        static char loadFilenameBuffer[256] = "text.txt";
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(LOAD_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", loadFilenameBuffer, sizeof(loadFilenameBuffer));

            if (ImGui::Button("Load", popUpButtonSize)) {
                LoadFromCsvFile(loadFilenameBuffer);
//                currentFileName = loadFilenameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void WindowClass::SaveToCsvFile(std::string_view fileName) {
        auto out = std::ofstream(fileName.data());
        if (out.is_open()) {
            out.close();
        }
    }

    void WindowClass::LoadFromCsvFile(std::string_view fileName) {
        auto in = std::ifstream(fileName.data());

        if (in.is_open()) {
            in.close();
        }
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Csv editor");
    }
} // namespace