#include "Window.h"

namespace ImGuiFinal {
    void Window::LoadTheme() {}

    void Window::SaveTheme() {}

    void Window::SettingsMenuBar() {
        static auto theme_menu_open = false;
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Settings")) {
                ImGui::MenuItem("Theme", nullptr, &theme_menu_open);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (theme_menu_open) {
            DrawColorSettings(&theme_menu_open);
        }
    }

    void Window::DrawColorSettings(bool *open) {}

    ImGuiStyle Window::DefaultColorStyle() {
        return ImGuiStyle{};
    }

    Window::~Window() {

    }
}