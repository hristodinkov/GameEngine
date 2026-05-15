//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#include "BenschmarkWriter.h"

#include <optional>
#include <xlnt/xlnt.hpp>
#include <fstream>
#include <sstream>

void writeExcel(const BenchmarkResult& result, const std::string& filename) {
    xlnt::workbook workbook;
    auto worksheet = workbook.active_sheet();

    worksheet.cell("A1").value("time");
    worksheet.cell("B1").value("frame");
    worksheet.cell("C1").value("fps");
    worksheet.cell("D1").value("frameTime");
    worksheet.cell("E1").value("satTests");
    worksheet.cell("F1").value("satTime");
    worksheet.cell("G1").value("collisions");
    worksheet.cell("I1").value("label");
    worksheet.cell("J1").value("objects");
    worksheet.cell("K1").value("useGrid");
    worksheet.cell("L1").value("seed");
    worksheet.cell("M1").value("cellSize");

    for (char col = 'A'; col <= 'M'; col++)
    {
        auto cell = worksheet.cell(std::string(1, col) + "1");
        cell.font(xlnt::font().bold(true));
        xlnt::color headerColor(xlnt::color(xlnt::rgb_color(0x00, 0xFF, 0x00)));
        cell.fill(xlnt::fill::solid(headerColor));
    }

    int row = 2;
    worksheet.cell("I" + std::to_string(row)).value(result.config.label);
    worksheet.cell("J" + std::to_string(row)).value(result.config.objectCount);
    worksheet.cell("K" + std::to_string(row)).value(result.config.useGrid ? 1 : 0);
    worksheet.cell("L" + std::to_string(row)).value(result.config.randomSeed);
    worksheet.cell("M" + std::to_string(row)).value(result.config.cellSize);
    for (const auto &s : result.samples)
    {
        worksheet.cell("A" + std::to_string(row)).value(s.time);
        worksheet.cell("B" + std::to_string(row)).value(s.frame);
        worksheet.cell("C" + std::to_string(row)).value(s.fps);
        worksheet.cell("D" + std::to_string(row)).value(s.frameTime);
        worksheet.cell("E" + std::to_string(row)).value(s.satTests);
        worksheet.cell("F" + std::to_string(row)).value(s.satTime);
        worksheet.cell("G" + std::to_string(row)).value(s.collisions);
        row++;
    }

    for (char col = 'A'; col <= 'M'; col++) {
        worksheet.column_properties(col).best_fit = true;
    }
    workbook.save(filename);
    std::cout << "Excel with " << filename << " has been created\n";
    //std::string cmd = "start \"\" \"" + filename + "\"";
    //system(cmd.c_str());
}

void writeCSV(const BenchmarkResult& result, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "label,objects,useGrid,seed\n";
    file << result.config.label << ","
         << result.config.objectCount << ","
         << (result.config.useGrid ? 1 : 0) << ","
         << result.config.randomSeed << "\n\n";

    file << "time,frame,fps,frameTime,satTests,satTime\n";


    for (const auto& s : result.samples) {
        file << s.time << ","
             << s.frame << ","
             << s.fps << ","
             << s.frameTime << ","
             << s.satTests << ","
             << s.satTime << "\n";
    }

    file.close();
}




