#pragma once

#include <string_view>
#include "imgui.h"

namespace ImGuiDesktop {
    class WindowClass {
    public:
        void Draw(std::string_view label);
    };

    void render(WindowClass &window_obj);
}
