//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#ifndef RAWENGINE_BENCHMARKRUNNER_H
#define RAWENGINE_BENCHMARKRUNNER_H

#pragma once


#include "../MyClasses/SceneManager.h"
#include "Benchmark.h"
#include "SpatialHashGrid.h"

struct GLFWwindow;

class BenchmarkRunner {
    public:
    BenchmarkRunner(SceneManager& sceneManager,GLFWwindow* window, core::Model& cubeModel):sceneManager(sceneManager),window(window),cubeModel(cubeModel){}
    BenchmarkResult run (const BenchmarkConfig& config);

    private:
    SceneManager &sceneManager;
    GLFWwindow* window;
    core::Model& cubeModel;
    SpatialHashGrid grid;

    void setupScene(const BenchmarkConfig& config);
};


#endif //RAWENGINE_BENCHMARKRUNNER_H