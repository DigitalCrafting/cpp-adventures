#include <imgui.h>
#include <valarray>
#include <filesystem>
#include <fstream>

#include "render.hpp"

namespace fs = std::filesystem;

namespace ImGuiCsvEditor {
    static constexpr std::string_view SAVE_POPUP_NAME = "Save File";
    static constexpr std::string_view LOAD_POPUP_NAME = "Load File";

    void WindowClass::Draw(std::string_view label) {
        constexpr static auto window_flags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
        constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);

        ImGui::Begin(label.data(), nullptr, window_flags);

        DrawSizeButtons();
        ImGui::Separator();
        DrawIoButtons();
        ImGui::Separator();
        DrawTable();

        ImGui::End();
    }

    void WindowClass::DrawSizeButtons() {
        auto user_added_rows = false;
        auto user_dropped_rows = false;
        auto user_added_cols = false;
        auto user_dropped_cols = false;

        auto slider_value_rows = numRows;
        auto slider_value_cols = numCols;

        ImGui::Text("Num Rows: ");
        ImGui::SameLine();

        if (ImGui::SliderInt("##numRows", &slider_value_rows, 0, maxNumRows)) {
            user_added_rows = slider_value_rows > numRows;
            user_dropped_rows = slider_value_rows < numRows;

            numRows = slider_value_rows;
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Row") && numRows < maxNumRows) {
            ++numRows;
            user_added_rows = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Drop Row") && numRows > 0) {
            --numRows;
            user_dropped_rows = true;
        }

        ImGui::Text("Num Cols: ");
        ImGui::SameLine();
        if (ImGui::SliderInt("##numCols", &slider_value_cols, 0 , maxNumCols)) {
            user_added_cols = slider_value_cols > numCols;
            user_dropped_cols = slider_value_cols < numCols;

            numCols = slider_value_cols;
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Col") && numCols < maxNumCols) {
            ++numCols;
            user_added_cols = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Drop Col") && numCols > 0) {
            --numCols;
            user_dropped_cols = true;
        }

        const auto num_rows_i32 = static_cast<std::int32_t>(data.size());
        if (user_added_rows) {
            for (auto row = num_rows_i32; row < numRows; ++row) {
                data.push_back(std::vector<float>(numCols, 0.0F));
            }
        } else if (user_added_cols) {
            for (std::int32_t row = 0; row < numRows; ++row) {
                const auto num_cols_i32 = static_cast<std::int32_t>(data[row].size());
                for (auto col = num_cols_i32; col < numCols; ++col) {
                    data[row].push_back(0.0F);
                }
            }
        } else if (user_dropped_rows) {
            for (auto row = num_rows_i32; row > numRows; --row) {
                data.pop_back();
            }
        } else if (user_dropped_cols) {
            for (std::int32_t row = 0; row < numRows; ++row) {
                const auto num_cols_i32 = static_cast<std::int32_t>(data[row].size());
                for (auto col = num_cols_i32; col > numCols; --col) {
                    data[row].pop_back();
                }
            }
        }
    }

    void WindowClass::DrawIoButtons() {}

    void WindowClass::DrawTable() {}

    void WindowClass::DrawSavePopup() {
        static char saveFilenameBuffer[256] = "text.txt";
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(SAVE_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", saveFilenameBuffer, sizeof(saveFilenameBuffer));

            if (ImGui::Button("Save", popUpButtonSize)) {
                SaveToCsvFile(saveFilenameBuffer);
//                currentFileName = saveFilenameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void WindowClass::DrawLoadPopup() {
        static char loadFilenameBuffer[256] = "text.txt";
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);

        if (ImGui::BeginPopupModal(LOAD_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", loadFilenameBuffer, sizeof(loadFilenameBuffer));

            if (ImGui::Button("Load", popUpButtonSize)) {
                LoadFromCsvFile(loadFilenameBuffer);
//                currentFileName = loadFilenameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void WindowClass::DrawValuePopup(const int row, const int col) {



    }

    template<typename T>
    void WindowClass::PlotCellValue(std::string_view formatter, const T value) {}

    void WindowClass::SetPopupLayout() {}

    void WindowClass::SaveToCsvFile(std::string_view fileName) {
        auto out = std::ofstream(fileName.data());
        if (out.is_open()) {
            out.close();
        }
    }

    void WindowClass::LoadFromCsvFile(std::string_view fileName) {
        auto in = std::ifstream(fileName.data());

        if (in.is_open()) {
            in.close();
        }
    }

    void render(WindowClass &window_obj) {
        window_obj.Draw("Csv editor");
    }
} // namespace