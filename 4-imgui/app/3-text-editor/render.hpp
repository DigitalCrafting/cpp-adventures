#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <cstring>

namespace ImGuiTextEditor {
    class WindowClass {
    public:
        static constexpr auto bufferSize = std::size_t{1024};
        static constexpr auto popUpFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        static constexpr auto popUpSize = ImVec2(300.0F, 100.0F);
        static constexpr auto popUpButtonSize = ImVec2(120.0F, 0.0F);
        static constexpr auto popUpPos = ImVec2(
                1280.0F / 2.0F - popUpSize.x / 2.0F,
                720.0F / 2.0F - popUpSize.y / 2.0F
        );

    public:
        WindowClass() : currentFileName({}) {
            std::memset(textBuffer, 0, bufferSize);
        }

        void Draw(std::string_view label);

    private:
        void DrawMenu();

        void DrawSavePopup();

        void DrawLoadPopup();

        void DrawContent();

        void DrawInfo();

        void SaveToFile(std::string_view fileName);

        void LoadToFile(std::string_view fileName);

        std::string GetFileExtension(std::string_view fileName);

    private:
        char textBuffer[bufferSize];
        std::string currentFileName;
    };

    void render(WindowClass &window_obj);
}
