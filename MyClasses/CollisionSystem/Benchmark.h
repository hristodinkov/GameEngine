//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#ifndef RAWENGINE_BENCHMARK_H
#define RAWENGINE_BENCHMARK_H

#pragma once
#include <string>
#include <vector>

struct BenchmarkConfig {
    int objectCount = 10;
    float testDurationSeconds = 10.0f;
    bool useGrid = false;
    unsigned int randomSeed = 47;
    std::string label = "baseline";
};

struct BenchmarkSample {
    float time;
    float fps;
    float frameTime;
    int satTests;
    double satTime;
};

struct BenchmarkResult {
    BenchmarkConfig config;
    std::vector<BenchmarkSample> samples;
};


#endif //RAWENGINE_BENCHMARK_H