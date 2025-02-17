#include <imgui.h>

#include "render.hpp"

namespace ImGuiWallClock {
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

    void WindowClass::DrawCircle(const float radius) {

    }

    void WindowClass::DrawClockHand(const float radius, const float theta, const ImColor color) {

    }

    void WindowClass::DrawAllHoursStrokes() {

    }

    void WindowClass::DrawAllMinutesStrokes() {

    }

    void WindowClass::DrawDigitalClock() {

    }

    void WindowClass::GetTime() {

    }

    std::tuple<float, float, float> WindowClass::GetTheta() {
        return {};
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Wall clock");
    }
} // namespace