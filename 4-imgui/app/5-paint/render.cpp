
#include <fstream>
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
        canvasPos = ImGui::GetCursorPos();
        const auto border_thickness = 1.5F;
        const auto button_size = ImVec2(canvasSize.x + 2.0F * border_thickness,
                                        canvasSize.y + 2.0F * border_thickness);

        ImGui::InvisibleButton("##canvas", button_size);

        const auto mouse_pos = ImGui::GetMousePos();
        const auto is_mouse_hovering = ImGui::IsItemHovered();

        if (is_mouse_hovering && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            const auto point = ImVec2(mouse_pos.x - canvasPos.x - border_thickness,
                                      mouse_pos.y - canvasPos.y - border_thickness);

            points.push_back(std::make_tuple(point, currentDrawColor, pointDrawSize));
        }

        auto *draw_list = ImGui::GetWindowDrawList();
        for (const auto &[point, color, size]: points) {
            const auto pos = ImVec2(
                    canvasPos.x + border_thickness + point.x,
                    canvasPos.y + border_thickness + point.y
            );
            draw_list->AddCircleFilled(pos, size, color);
        }

        const auto border_min = canvasPos;
        const auto border_max = ImVec2(
                canvasPos.x + button_size.x - border_thickness,
                canvasPos.y + button_size.y - border_thickness
                );

        draw_list->AddRect(border_min,
                           border_max,
                           IM_COL32(255,255,255,255),
                           0.0F,
                           ImDrawFlags_RoundCornersAll,
                           border_thickness);
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

    void WindowClass::DrawSavePopup() {
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(SAVE_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", filenameBuffer, sizeof(filenameBuffer));

            if (ImGui::Button("Save", popUpButtonSize)) {
                SaveToImageFile(filenameBuffer);
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
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(LOAD_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", filenameBuffer, sizeof(filenameBuffer));

            if (ImGui::Button("Load", popUpButtonSize)) {
                LoadFromImageFile(filenameBuffer);
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void WindowClass::SaveToImageFile(std::string_view filename) {
        auto out = std::ofstream(filename.data(), std::ios::binary);

        if (!out || !out.is_open())
            return;

        const auto point_count = points.size();
        out.write(reinterpret_cast<const char*>(&point_count), sizeof (point_count));

        for (const auto &[point, color, size] : points) {
            out.write(reinterpret_cast<const char*>(&point), sizeof(point));
            out.write(reinterpret_cast<const char*>(&color), sizeof(color));
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        }

        out.close();
    }

    void WindowClass::LoadFromImageFile(std::string_view filename) {
        auto in = std::ifstream(filename.data(), std::ios::binary);

        if (!in || !in.is_open())
            return;

        auto point_count = std::size_t {0};
        in.read(reinterpret_cast<char*>(&point_count), sizeof(point_count));

        for (std::size_t i = 0; i < point_count; ++i) {
            auto point = ImVec2{};
            auto color = ImColor{};
            auto size = float{};

            in.read(reinterpret_cast<char*>(&point), sizeof(point));
            in.read(reinterpret_cast<char*>(&color), sizeof(color));
            in.read(reinterpret_cast<char*>(&size), sizeof(size));

            points.push_back(std::make_tuple(point, color, size));
        }

        in.close();
    }

    void WindowClass::ClearCanvas() {
        points.clear();
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Paint");
    }
} // namespace