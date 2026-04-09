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

        // deterministic random position
        glm::vec2 r = rand2(glm::vec2(i, config.randomSeed), 3.0f);

        // axis = 0, 1, or 2
        int axis = int(r.x * 3.0f) % 3;

        // deterministic speed
        float speed = 0.5f + r.y * 2.0f;

        // deterministic phase
        glm::vec2 rPhase = rand2(glm::vec2(i, config.randomSeed), 4.0f);
        float phase = rPhase.x * 6.283185f;

        // amplitude
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
        lastTime = now;

        if (elapsed >= config.testDurationSeconds)
            break;

        // update engine
        sceneManager.update(deltaTime);

        // collect metrics
        float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
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
