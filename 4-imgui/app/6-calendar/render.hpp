#pragma once

#include <imgui.h>
#include <cstdint>
#include <string_view>
#include <string>
#include <vector>
#include <cstring>
#include <array>
#include <chrono>
#include <map>

namespace ImGuiCalendar {
    class WindowClass {
    public:
        static constexpr auto meetingWindowFlags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        static constexpr auto meetingWindowSize = ImVec2(300.0F, 100.0F);
        static constexpr auto meetingWindowButtonSize = ImVec2(120.0F, 0.0F);
        static constexpr auto meetingWindowPos = ImVec2(1280.0F / 2.0F - meetingWindowSize.x / 2.0F,
                                                        720.0F / 2.0F - meetingWindowSize.y / 2.0F);

        static constexpr auto monthNames = std::array<std::string_view, 12U>{
                "January",
                "February",
                "March",
                "April",
                "May",
                "June",
                "July",
                "August",
                "September",
                "October",
                "November",
                "December"
        };

        static constexpr auto minYear = 2000U;
        static constexpr auto maxYear = 2038U;

        struct Meeting {
            std::string name;

            void Serialize(std::ofstream &out) const;
            static Meeting Deserialize(std::ifstream &in);

            constexpr bool operator==(const Meeting &rhs) const {
                return name == rhs.name;
            }
        };

    public:
        WindowClass(): meetings({}){};

        void Draw(std::string_view label);

        void LoadMeetingsFromFile(std::string_view filename);
        void SaveMeetingsToFile(std::string_view filename);

    private:
        void DrawDateCombo();
        void DrawCalendar();
        void DrawAddMeetingsWindow();
        void DrawMeetingsList();

        void UpdateSelectedDateVariables();

    private:
        int selectedDay = 1;
        int selectedMonth = 1;
        int selectedYear = 2025;
        std::chrono::year_month_day selectedDate;

        bool addMeetingWindowOpen = false;

        float calendarFontSize = 2.0F;

        std::map<std::chrono::year_month_day, std::vector<Meeting>> meetings;
    };

    void render(WindowClass &window_obj);
}
