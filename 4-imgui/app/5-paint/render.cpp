
#include "render.hpp"

namespace ImGuiPaint {
    static constexpr std::string_view SAVE_POPUP_NAME = "Save Image";
    static constexpr std::string_view LOAD_POPUP_NAME = "Load Image";

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
        DrawCanvas();

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
            ClearCanvas();
        }

        DrawColorButtons();
        DrawSizeSettings();

        DrawSavePopup();
        DrawLoadPopup();
    }

    void WindowClass::DrawCanvas() {

    }

    void WindowClass::DrawColorButtons() {
        const auto selected_red = currentDrawColor == ImColor(255, 0, 0);
        const auto selected_green = currentDrawColor == ImColor(0, 255, 0);
        const auto selected_blue = currentDrawColor == ImColor(0, 0, 255);
        const auto selected_white = currentDrawColor == ImColor(255, 255, 255);
        const auto none_preset_color = !selected_red && !selected_green && !selected_blue && !selected_white;

        constexpr static auto orange = ImVec4(1.0F, 0.5F, 0.0F, 1.0F);

        if (selected_red) {
            ImGui::PushStyleColor(ImGuiCol_Button, orange);
        }
        if (ImGui::Button("Red")) {
            currentDrawColor = ImColor(255, 0, 0);
        }
        if (selected_red) {
            ImGui::PopStyleColor();
        }

        ImGui::SameLine();

        if (selected_green) {
            ImGui::PushStyleColor(ImGuiCol_Button, orange);
        }
        if (ImGui::Button("Green")) {
            currentDrawColor = ImColor(0, 255, 0);
        }
        if (selected_green) {
            ImGui::PopStyleColor();
        }

        ImGui::SameLine();

        if (selected_blue) {
            ImGui::PushStyleColor(ImGuiCol_Button, orange);
        }
        if (ImGui::Button("Blue")) {
            currentDrawColor = ImColor(0, 0, 255);
        }
        if (selected_blue) {
            ImGui::PopStyleColor();
        }

        ImGui::SameLine();

        if (selected_white) {
            ImGui::PushStyleColor(ImGuiCol_Button, orange);
        }
        if (ImGui::Button("White")) {
            currentDrawColor = ImColor(255, 255, 255);
        }
        if (selected_white) {
            ImGui::PopStyleColor();
        }

        ImGui::SameLine();

        if (none_preset_color) {
            ImGui::PushStyleColor(ImGuiCol_Button, orange);
        }
        if (ImGui::Button("Choose")) {
            ImGui::OpenPopup("Color Picker");
        }
        if (ImGui::BeginPopup("Color Picker")) {
            ImGui::ColorPicker3("##picker",
                                reinterpret_cast<float *>(&currentDrawColor));
            ImGui::EndPopup();
        }
        if (none_preset_color) {
            ImGui::PopStyleColor();
        }
    }

    void WindowClass::DrawSizeSettings() {
        ImGui::Text("Draw Size");
        ImGui::SameLine();
        ImGui::PushItemWidth(canvasSize.x - ImGui::GetCursorPosX());
        ImGui::SliderFloat("##drawSize", &pointDrawSize, 1.0F, 10.0F);
        ImGui::PopItemWidth();
    }

    void WindowClass::DrawSavePopup() {}

    void WindowClass::DrawLoadPopup() {}

    void WindowClass::SaveToImageFile(std::string_view filename) {}

    void WindowClass::LoadFromImageFile(std::string_view filename) {}

    void WindowClass::ClearCanvas() {
        points.clear();
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Paint");
    }
} // namespace