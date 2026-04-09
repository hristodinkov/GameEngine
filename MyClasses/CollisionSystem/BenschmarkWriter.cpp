//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#include "BenschmarkWriter.h"

#include <optional>
#include <xlnt/xlnt.hpp>

void writeExcel(const BenchmarkResult& result, const std::string& filename) {
    xlnt::workbook workbook;
    auto worksheet = workbook.active_sheet();

    worksheet.cell("A1").value("label");
    worksheet.cell("B1").value("objects");
    worksheet.cell("C1").value("useGrid");
    worksheet.cell("D1").value("seed");
    worksheet.cell("E1").value("time");
    worksheet.cell("F1").value("fps");
    worksheet.cell("G1").value("frameTime");
    worksheet.cell("H1").value("satTests");
    worksheet.cell("I1").value("satTime");

    for (char col = 'A'; col <= 'I'; col++)
    {
        auto cell = worksheet.cell(std::string(1, col) + "1");
        cell.font(xlnt::font().bold(true));
        xlnt::color headerColor(xlnt::color(xlnt::rgb_color(0x00, 0xFF, 0x00)));
        cell.fill(xlnt::fill::solid(headerColor));
    }

    int row = 2;
    for (const auto &s : result.samples)
    {
        worksheet.cell("A" + std::to_string(row)).value(result.config.label);
        worksheet.cell("B" + std::to_string(row)).value(result.config.objectCount);
        worksheet.cell("C" + std::to_string(row)).value(result.config.useGrid ? 1 : 0);
        worksheet.cell("D" + std::to_string(row)).value(result.config.randomSeed);
        worksheet.cell("E" + std::to_string(row)).value(s.time);
        worksheet.cell("F" + std::to_string(row)).value(s.fps);
        worksheet.cell("G" + std::to_string(row)).value(s.frameTime);
        worksheet.cell("H" + std::to_string(row)).value(s.satTests);
        worksheet.cell("I" + std::to_string(row)).value(s.satTime);
        row++;
    }

    for (char col = 'A'; col <= 'I'; col++) {
        worksheet.column_properties(col).best_fit = true;
    }
    workbook.save(filename);
    std::string cmd = "start \"\" \"" + filename + "\"";
    system(cmd.c_str());
}



