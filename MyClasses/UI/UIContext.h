//
// Created by Hristo Dinkov on 24.8.2026 г..
//

#ifndef RAWENGINE_UICONTEXT_H
#define RAWENGINE_UICONTEXT_H

#pragma once
#include "../MyClasses/SceneManagement/SceneManager.h"
#include "../MyClasses/CollisionSystem/BenschmarkWriter.h"
#include "../core/mesh.h"

struct UIContext {
    SceneManager& sceneManager;
    std::shared_ptr<GameObject> cube1, cube2;
    float deltaTime, fps;
    double averageTimePerSAT;
    BenchmarkConfig& uiConfig;
    BenchmarkConfig& benchmarkConfig;
    int& currentPostProcessingMode;
    float& pixels;
    float& kernelCenterValueMatrix;
    core::Model cubeModel, tetraModel;
};


#endif //RAWENGINE_UICONTEXT_H