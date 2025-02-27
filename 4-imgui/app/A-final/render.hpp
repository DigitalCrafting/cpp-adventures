#pragma once

#include <string_view>
#include <imgui.h>
#include <string>
#include <vector>

#include "Clock.h"
#include "Calendar.h"
#include "fmt/format.h"
#include "CsvEditor.h"

namespace ImGuiFinal {
    class WindowClass {
    public:
        constexpr static auto numIcons = std::uint32_t{2};

        struct Icon {
            Icon(std::string_view label_, Window *window_): label(label_), position(ImVec2{}), window(window_), popupOpen(false), clickCount(0) {};

            void Draw();
            std::string label;
            ImVec2 position;
            bool popupOpen;
            std::uint32_t clickCount;
            Window *window = nullptr;
        };

    public:
        WindowClass() : icons(), clock({}), calendar(), csvEditor() {
            icons.reserve(numIcons);
            icons.emplace_back("Calendar", &calendar);
            icons.emplace_back("CsvEditor", &csvEditor);
        };

        void Draw(std::string_view label);

    private:
        void DrawDesktop();
        void DrawTaskbar();

        void ShowIconList(bool *open = nullptr);

    private:
        std::vector<Icon> icons;
        Clock clock;

        Calendar calendar;
        CsvEditor csvEditor;
    };

    void render(WindowClass &window_obj);
}
