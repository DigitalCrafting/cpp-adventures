#include "CsvEditor.h"

#include <imgui.h>
#include <filesystem>
#include <fstream>

#include "fmt/format.h"

namespace fs = std::filesystem;

namespace ImGuiFinal {
    static constexpr std::string_view SAVE_POPUP_NAME = "Save File";
    static constexpr std::string_view LOAD_POPUP_NAME = "Load File";
    static constexpr std::string_view VALUE_POPUP_NAME = "Change Value";

    void CsvEditor::Draw(std::string_view label, bool *open) {
        constexpr static auto window_flags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
        constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);

        ImGui::Begin(label.data(), open, window_flags);

        DrawSizeButtons();
        ImGui::Separator();
        DrawIoButtons();
        ImGui::Separator();
        DrawTable();

        ImGui::End();
    }

    void CsvEditor::DrawSizeButtons() {
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
        if (ImGui::SliderInt("##numCols", &slider_value_cols, 0, maxNumCols)) {
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

    void CsvEditor::DrawIoButtons() {
        if (ImGui::Button("Save")) {
            ImGui::OpenPopup(SAVE_POPUP_NAME.data());
        }

        ImGui::SameLine();

        if (ImGui::Button("Load")) {
            ImGui::OpenPopup(LOAD_POPUP_NAME.data());
        }

        ImGui::SameLine();

        if (ImGui::Button("Clear")) {
            data.clear();
            numRows = 0;
            numCols = 0;
        }

        DrawSavePopup();
        DrawLoadPopup();
    }

    void CsvEditor::DrawTable() {
        constexpr static auto table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter;

        static auto row_clicked = 0;
        static auto col_clicked = 0;

        if (numCols == 0) {
            return;
        }

        ImGui::BeginTable("Table", numCols, table_flags);

        for (std::int32_t col = 0; col < numCols; ++col) {
            const auto col_name = fmt::format("{}", 'A' + col);
            ImGui::TableSetupColumn(col_name.data(), ImGuiTableColumnFlags_WidthFixed, 1280.0F / static_cast<float>(numCols));
        }

        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (std::int32_t col = 0; col < numCols; ++col) {
            PlotCellValue("%c", 'A' + col);
        }

        for (std::int32_t row = 0; row < numRows; ++row) {
            for (std::int32_t col = 0; col < numCols; ++col) {
                PlotCellValue("%f", data[row][col]);
                if (ImGui::IsItemClicked()) {
                    ImGui::OpenPopup(VALUE_POPUP_NAME.data());
                    row_clicked = row;
                    col_clicked = col;
                } else if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Cell: (%d, %d)", row, col);
                }
            }
            ImGui::TableNextRow();
        }

        DrawValuePopup(row_clicked, col_clicked);

        ImGui::EndTable(); // Table
    }

    void CsvEditor::DrawSavePopup() {
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        SetPopupLayout();
        if (ImGui::BeginPopupModal(SAVE_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", filenameBuffer, sizeof(filenameBuffer));

            if (ImGui::Button("Save", popUpButtonSize)) {
                SaveToCsvFile(filenameBuffer);
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void CsvEditor::DrawLoadPopup() {
        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        SetPopupLayout();
        if (ImGui::BeginPopupModal(LOAD_POPUP_NAME.data(), nullptr, popUpFlags)) {
            ImGui::InputText("Filename", filenameBuffer, sizeof(filenameBuffer));

            if (ImGui::Button("Load", popUpButtonSize)) {
                LoadFromCsvFile(filenameBuffer);
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void CsvEditor::DrawValuePopup(const int row, const int col) {
        static char buffer[64] = {'\0'};

        const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

        SetPopupLayout();
        if (ImGui::BeginPopupModal(VALUE_POPUP_NAME.data(), nullptr, popUpFlags)) {
            const auto label = fmt::format("##{}_{}", row, col);
            ImGui::InputText(label.data(), buffer, sizeof(buffer));

            if (ImGui::Button("Save", popUpButtonSize)) {
                data[row][col] = std::stof(buffer);
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    template<typename T>
    void CsvEditor::PlotCellValue(std::string_view formatter, const T value) {
        ImGui::TableNextColumn();
        ImGui::Text(formatter.data(), value);
    }

    void CsvEditor::SetPopupLayout() {
        ImGui::SetNextWindowSize(popUpSize);
        ImGui::SetNextWindowPos(popUpPos);
    }

    void CsvEditor::SaveToCsvFile(std::string_view fileName) {
        auto out = std::ofstream(fileName.data());

        if (!out || !out.is_open()) {
            return;
        }

        for (std::int32_t row = 0; row < numRows; ++row) {
            for (std::int32_t col = 0; col < numCols; ++col) {
                out << data[row][col];
                out << ',';
            }
            out << '\n';
        }

        out.close();
    }

    void CsvEditor::LoadFromCsvFile(std::string_view fileName) {
        auto in = std::ifstream(fileName.data());

        if (!in || !in.is_open()) {
            return;
        }

        data.clear();

        auto line = std::string {};
        auto num_rows = 0U;

        while (std::getline(in, line)) {
            auto ss = std::istringstream(line);
            auto row = std::vector<float>{};
            auto value = std::string{};

            while (std::getline(ss, value, ',')) {
                row.push_back(std::stof(value));
            }

            data.push_back(row);
            ++num_rows;
        }

        in.close();

        numRows = num_rows;
        if (numRows > 0U) {
            numCols = static_cast<std::int32_t>(data[0].size());
        } else {
            numCols = 0U;
        }
    }

    void render(CsvEditor &window_obj) {
        window_obj.Draw("Csv editor");
    }
}
