#include <imgui.h>
#include <iostream>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawMenu();
    ImGui::Separator();
    DrawContent();
    ImGui::Separator();
    DrawActions();
    ImGui::Separator();
    DrawFilter();

    ImGui::End();
}

void WindowClass::DrawMenu() {
    if (ImGui::Button("Go Up")) {
        if (currentPath.has_parent_path()) {
            currentPath = currentPath.parent_path();
        }
    }

    ImGui::SameLine();

    ImGui::Text("Current directory: %s", currentPath.string().c_str());
}

void WindowClass::DrawContent() {
    ImGui::Text("DrawContent");
}

void WindowClass::DrawActions() {
    ImGui::Text("DrawActions");
}

void WindowClass::DrawFilter() {
    ImGui::Text("DrawFilter");
}


void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}