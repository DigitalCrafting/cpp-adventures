#include <imgui.h>
#include <valarray>
#include <filesystem>
#include <fstream>

#include "render.hpp"

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

    void WindowClass::DrawContent() {
        static constexpr auto inputTextSize = ImVec2(1200.0F, 625.0F);
        static constexpr auto lineNumberSize = ImVec2(30.0F, inputTextSize.y);
        static constexpr auto inputTextFlags =
                ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll;

        const float lineHeight = ImGui::GetTextLineHeightWithSpacing();

        ImGui::BeginChild("LineNumbers", lineNumberSize, false, ImGuiWindowFlags_NoScrollbar);
        const auto line_count = std::count(textBuffer, textBuffer + bufferSize, '\n') + 1;

        /* Smaller vertical spacing, so that line numbers actually correspond to lines in InputTextMultiline */
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0F, 0.5F));

        for (auto i = 1; i <= line_count; ++i) {
            ImGui::Text("%d", i);
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginChild("Editor", inputTextSize, false, ImGuiWindowFlags_NoScrollbar);
        ImGui::InputTextMultiline("###inputField", textBuffer, bufferSize, inputTextSize, inputTextFlags);
        ImGui::EndChild();
    }

    void WindowClass::DrawInfo() {
        if (currentFileName.size() == 0) {
            ImGui::Text("No File Opened!");
            return;
        }

        const auto file_extension = GetFileExtension(currentFileName);
        ImGui::Text("Opened file %s | File extension %s", currentFileName.data(), file_extension.data());
    }

    void WindowClass::SaveToFile(std::string_view fileName) {
        auto out = std::ofstream(fileName.data());
        if (out.is_open()) {
            out << textBuffer;
            out.close();
        }
    }

    void WindowClass::LoadFromFile(std::string_view fileName) {
        auto in = std::ifstream(fileName.data());

        if (in.is_open()) {
            auto buffer = std::stringstream{};
            buffer << in.rdbuf();
            std::memcpy(textBuffer, buffer.str().data(), bufferSize);
            in.close();
        }
    }

    std::string WindowClass::GetFileExtension(std::string_view fileName) {
        const auto file_path = fs::path(fileName);
        return file_path.extension().string();
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Text editor");
    }
} // namespace