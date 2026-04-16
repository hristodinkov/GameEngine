//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#include "BenchmarkRunner.h"

#include <cstdlib>
#include <chrono>
#include <glm/vec3.hpp>


#include "ConvexCollider.h"
#include "../GameObject.h"
#include "../Random.h"
#include "../SinMovement.h"

#include <GLFW/glfw3.h>

float gBenchmarkTime = 0.0f;

void BenchmarkRunner::setupScene(const BenchmarkConfig& config) {
    std::srand(config.randomSeed);

    auto scene = sceneManager.createScene("Benchmark");
    sceneManager.setActiveScene("Benchmark");

    for (int i = 0; i < config.objectCount; ++i) {
        auto go = scene->addObject(std::make_shared<GameObject>("Cube" + std::to_string(i)));
        go->model = cubeModel;

        glm::vec2 r = rand2(glm::vec2(i, config.randomSeed), 3.0f);

        int axis = int(r.x * 3.0f) % 3;

        float speed = 0.5f + r.y * 2.0f;

        glm::vec2 rPhase = rand2(glm::vec2(i, config.randomSeed), 4.0f);
        float phase = rPhase.x * 6.283185f;

        float amplitude = 2.0f;

        go->addBehavior(std::make_shared<SinMovement>(axis, speed, amplitude, phase));

        go->collider = std::make_shared<ConvexCollider>(
            go->model->getAllVertices(),
            go->model->getAllIndices(),
            go->getWorldTransform()
        );
    }

    //ToDo
    // later: toggle grid here based on config.useGrid
}

BenchmarkResult BenchmarkRunner::run(const BenchmarkConfig& config) {
    setupScene(config);

    BenchmarkResult result;
    result.config = config;

    double startTime = glfwGetTime();
    double lastTime = startTime;

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double elapsed = now - startTime;
        float deltaTime = static_cast<float>(now - lastTime);
        float fps;
        lastTime = now;

        if (elapsed >= config.testDurationSeconds)
            break;

        sceneManager.update(deltaTime);

        auto activeScene = sceneManager.getActiveScene();
        std::vector<std::pair<GameObject*, GameObject*>> pairs;

        // Use grid or brute force depending on config
        if (config.useGrid) {
            grid.buildGrid(activeScene->objects);
            pairs = grid.computePairs();
        } else {
            pairs = sceneManager.computeBruteForcePairs();
        }

        // Reset SAT stats for this frame
        sceneManager.resetSatStats();

        // Run SAT on all pairs
        for (auto& [A, B] : pairs) {
            sceneManager.runSAT(A, B);
        }


        if(deltaTime > 0.0f) {
            fps = 1.0f / deltaTime;
        }else {
            fps = 0.0f;
        }
        int satCount = sceneManager.getSatCount();
        double satTime = sceneManager.getSatTime();

        BenchmarkSample s;
        s.time = static_cast<float>(elapsed);
        s.fps = fps;
        s.frameTime = deltaTime;
        s.satTests = satCount;
        s.satTime = satTime;
        result.samples.push_back(s);

        gBenchmarkTime = float(elapsed);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return result;
}
