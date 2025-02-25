#pragma once

#include <filesystem>
#include <string_view>
#include <imgui.h>
#include <SimpleIni.h>
#include <implot.h>

namespace fs = std::filesystem;

namespace ImGuiFinal {
    class Window {
    public:
        constexpr static auto mainWindowFlags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto mainWindowSize = ImVec2(1280.0F, 40.0F);
        constexpr static auto mainWindowPos = ImVec2(0.0F, 680.0F);

    public:
        Window(): ini(CSimpleIniA()){};
        virtual ~Window();

        void SettingsMenuBar();
        void LoadTheme();
        void SaveTheme();

        virtual void Draw(std::string_view label, bool *open = nullptr) = 0;

    protected:
        void DrawColorSettings(bool *open = nullptr);
        static ImGuiStyle DefaultColorStyle();

        CSimpleIniA ini;
    };
}
