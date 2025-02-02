#include <imgui.h>
#include <valarray>
#include <implot.h>
#include <filesystem>

#include "render.hpp"
#include "fmt/format.h"

namespace fs = std::filesystem;

namespace ImGuiTextEditor {
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

        DrawMenu();
        DrawContent();
        DrawInfo();

        ImGui::End();
    }

    void WindowClass::DrawMenu() {
        const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
        const auto s_pressed = ImGui::IsKeyPressed(ImGuiKey_S);
        const auto l_pressed = ImGui::IsKeyPressed(ImGuiKey_L);

        if (ImGui::Button("Save") || (ctrl_pressed && s_pressed)) {
            ImGui::OpenPopup(SAVE_POPUP_NAME.data());
        }

        ImGui::SameLine();

        if (ImGui::Button("Load") || (ctrl_pressed && l_pressed)) {
            ImGui::OpenPopup(LOAD_POPUP_NAME.data());
        }

        ImGui::SameLine();

        if (ImGui::Button("Clear")) {
            std::memset(textBuffer, 0, bufferSize);
        }

        DrawSavePopup();
        DrawLoadPopup();
    }

    void WindowClass::DrawSavePopup() {
        static char saveFilenameBuffer[256] = "text.txt";
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(SAVE_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", saveFilenameBuffer, sizeof(saveFilenameBuffer));

            if (ImGui::Button("Save", popUpButtonSize)) {
                SaveToFile(saveFilenameBuffer);
                currentFileName = saveFilenameBuffer;
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
                LoadFromFile(loadFilenameBuffer);
                currentFileName = loadFilenameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void WindowClass::DrawContent() {}

    void WindowClass::DrawInfo() {}

    void WindowClass::SaveToFile(std::string_view fileName) {}

    void WindowClass::LoadFromFile(std::string_view fileName) {}

    std::string WindowClass::GetFileExtension(std::string_view fileName) {}

    void render(WindowClass &window_obj) {
        window_obj.Draw("Text editor");
    }
} // namespace