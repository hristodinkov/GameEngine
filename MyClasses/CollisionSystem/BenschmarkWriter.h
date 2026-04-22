//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#ifndef RAWENGINE_BENSCHMARKWRITER_H
#define RAWENGINE_BENSCHMARKWRITER_H
#pragma once
#include "Benchmark.h"
#include <string>

void writeExcel(const BenchmarkResult& result, const std::string& filename);


#endif //RAWENGINE_BENSCHMARKWRITER_H