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

    DrawSelection();
    DrawPlot();

    ImGui::End();
}

void WindowClass::DrawSelection() {
    for (const auto func_name : functionNames) {
        const auto curr_function = functionNameMapping(func_name);
        auto selected = selectedFunctions.count(curr_function) == 1;

        if (ImGui::Checkbox(func_name.data(), &selected)) {
            if (selected) {
                selectedFunctions.insert(curr_function);
            } else {
                selectedFunctions.erase(curr_function);
            }
        }
    }
}

void WindowClass::DrawPlot() {

}

WindowClass::Function WindowClass::functionNameMapping(std::string_view function_name) {
    if (std::string_view{"sin(x)"} == function_name) {
        return WindowClass::Function::SIN;
    }

    if (std::string_view{"cos(x)"} == function_name) {
        return WindowClass::Function::COS;
    }

    return WindowClass::Function::NONE;
}

double WindowClass::evaluateFunction(const WindowClass::Function function, const double x) {

}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Function Plotter");
}