#include <imgui.h>
#include <iostream>
#include <cstring>

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
    for (const auto &entry : fs::directory_iterator(currentPath)) {
        const auto is_selected = entry.path() == selectedEntry;
        const auto is_directory = entry.is_directory();
        const auto is_file = entry.is_regular_file();
        auto entry_name = entry.path().filename().string();

        if (is_directory) {
            entry_name = "[D] " + entry_name;
        } else if(is_file) {
            entry_name = "[F] " + entry_name;
        }

        if(ImGui::Selectable(entry_name.c_str(), is_selected)) {
            if (is_directory) {
                currentPath /= entry.path().filename();
            }

            selectedEntry = entry.path();
        }
    }
}

void WindowClass::DrawActions() {
    ImGui::Text("DrawActions");
}

void WindowClass::DrawFilter() {
    static char extension_filter[16] = {"\0"};

    ImGui::Text("Filter by extension");
    ImGui::SameLine();
    ImGui::InputText("###inFilter", extension_filter, sizeof(extension_filter));

    if (std::strlen(extension_filter) == 0) {
        return;
    }

    auto filtered_file_count = (unsigned int){0};
    for (const auto &entry : fs::directory_iterator(currentPath)) {
        if (!fs::is_regular_file(entry)) {
            continue;
        }

        if (entry.path().extension().string() == extension_filter) {
            ++filtered_file_count;
        }
    }

    ImGui::Text("Number of files: %u", filtered_file_count);
}


void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}