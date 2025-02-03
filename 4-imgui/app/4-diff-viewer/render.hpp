#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <cstring>

namespace ImGuiDiffViewer {
    class WindowClass {
    public:
        void Draw(std::string_view label);
    };

    void render(WindowClass &window_obj);
}
