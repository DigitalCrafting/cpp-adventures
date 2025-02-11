#pragma once

#include <imgui.h>
#include <cstdint>
#include <string_view>
#include <string>
#include <vector>
#include <cstring>

namespace ImGuiCalendar {
    class WindowClass {
    public:
        using PointData = std::tuple<ImVec2, ImColor, float>;
        static constexpr auto meetingWindowFlags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        static constexpr auto meetingWindowSize = ImVec2(300.0F, 100.0F);
        static constexpr auto meetingWindowButtonSize = ImVec2(120.0F, 0.0F);
        static constexpr auto meetingWindowPos = ImVec2(1280.0F / 2.0F - meetingWindowSize.x / 2.0F,
                                                720.0F / 2.0F - meetingWindowSize.y / 2.0F);

    public:
        void Draw(std::string_view label);
    };

    void render(WindowClass &window_obj);
}
