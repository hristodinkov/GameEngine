//
// Created by Hristo Dinkov on 24.8.2026 г..
//
#include <glad/glad.h>
#include "UIPanels.h"
#include "../MyClasses/GlobalVariables.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>  
#include <unordered_map>

void drawCubeControlsPanel(UIContext& ctx) {
    if (ImGui::Begin("Cube Controls")) {
        ImGui::Text("Move Cube 1");
        if (ImGui::SliderFloat3("Cube1 Pos", glm::value_ptr(ctx.cube1->transform.position), -10.0f, 10.0f))
            ctx.cube1->setPos(ctx.cube1->transform.position);

        ImGui::Separator();

        ImGui::Text("Move Cube 2");
        if (ImGui::SliderFloat3("Cube2 Pos", glm::value_ptr(ctx.cube2->transform.position), -10.0f, 10.0f))
            ctx.cube2->setPos(ctx.cube2->transform.position);
    }
    ImGui::End();
}

void drawBenchmarkPanel(UIContext& ctx) {
    if (ImGui::Begin("Benchmark")) {
        ImGui::InputInt("Object Count", &bmObjectCount);
        ImGui::InputInt("Duration (frames)", &bmDuration);
        ImGui::Checkbox("Use Grid", &useGrid);
        ImGui::InputText("Label", bmLabel, sizeof(bmLabel));
        ImGui::Checkbox("Use Tetrahedron", &useTetrahedron);
        ImGui::Checkbox("Camera look away", &bmCamera);

        ctx.uiConfig.objectCount = bmObjectCount;
        ctx.uiConfig.testDurationFrames = bmDuration;
        ctx.uiConfig.useGrid = useGrid;
        ctx.uiConfig.label = bmLabel;
        ctx.uiConfig.randomSeed = 47;

        if (ImGui::Button("Spawn Objects")) {
            if (useTetrahedron)
                ctx.sceneManager.spawnCubesInScene(ctx.uiConfig.objectCount, ctx.uiConfig.randomSeed, ctx.tetraModel);
            else
                ctx.sceneManager.spawnCubesInScene(ctx.uiConfig.objectCount, ctx.uiConfig.randomSeed, ctx.cubeModel);
        }

        if (ImGui::Button("Run test")) {
            filename = std::string("test_") + bmLabel + ".xls";
            ctx.uiConfig.cellSize = grid.cellSize;

            if (bmCamera) {
                camera.xRotation = 0.0f;
                camera.yRotation = 180.0f;
                camera.transform.rotation = glm::vec3(
                    glm::radians(camera.xRotation),
                    glm::radians(camera.yRotation),
                    0.0f
                );
            }
            camera.transform.updateModelMatrix();

            if (isFileLocked(filename)) {
                lockedFilename = filename;
                showFileLockedPopup = true;
            } else if (benchmarkRunning) {
                testIsRunning = true;
            } else {
                ctx.benchmarkConfig = ctx.uiConfig;
                benchmarkRunning = true;
                benchmarkStartTime = glfwGetTime();
                frozenUseGrid = useGrid;

                benchmarkResult = BenchmarkResult();
                benchmarkResult.config = ctx.benchmarkConfig;

                ctx.sceneManager.resetSatStats();
            }
        }
    }
    ImGui::End();

    if (testIsRunning) {
        ImGui::OpenPopup("Test is running!");
    }

    if (ImGui::BeginPopupModal("Test is running!", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("A test is currently running.");
        ImGui::Text("Please wait until the test is complete.");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            testIsRunning = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (showFileLockedPopup) {
        ImGui::OpenPopup("File Locked");
    }

    if (ImGui::BeginPopupModal("File Locked", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("The file '%s' is currently open in another program.", lockedFilename.c_str());
        ImGui::Text("Please close it before running the test again or set a new name for the test.");

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            showFileLockedPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void drawPerformancePanel(UIContext& ctx) {
    if (ImGui::Begin("Performance")) {
        ImGui::Text("FPS: %.1f", ctx.fps);
        ImGui::Text("Frame Time: %.3f ms", ctx.deltaTime * 1000.0f);

        ImGui::Separator();
        ImGui::Text("SAT Tests This Frame: %d", ctx.sceneManager.getSatCount());
        ImGui::Text("SAT Time: %.6f s", ctx.sceneManager.getSatTime());
        ImGui::Text("Avg Time per SAT: %.9f s", ctx.averageTimePerSAT);
    }
    ImGui::End();
}

void drawGridSettingsPanel(UIContext& ctx) {
    static const float cellSizes[] = { 200, 100, 50, 25, 10, 5, 1, 0.5f, 0.1f, 0.05f, 0.01f, 0.005f };

    if (ImGui::Begin("Grid Settings")) {
        for (float size : cellSizes) {
            std::string label = "CellSize = " + std::to_string(size);
            if (ImGui::Button(label.c_str()))
                grid.cellSize = size;
        }
    }
    ImGui::End();
}

void drawPostProcessingPanel(UIContext& ctx) {
    if (ImGui::Begin("Post Processing")) {
        ImGui::Combo("Effect", &ctx.currentPostProcessingMode,
            "None\0Grayscale\0Invert\0EdgeDetection\0Pixelization");

        if (ctx.currentPostProcessingMode == 3)
            ImGui::SliderFloat("Kernel Central Value", &ctx.kernelCenterValueMatrix, 0, -20);

        if (ctx.currentPostProcessingMode == 4)
            ImGui::SliderFloat("Pixels", &ctx.pixels, 1, 1024);
    }
    ImGui::End();
}

void drawScenesPanel(UIContext& ctx) {
    if (ImGui::Begin("Scenes")) {
        std::string active = ctx.sceneManager.getActiveScene()->getName();
        auto sceneNames = ctx.sceneManager.getSceneNames();

        for (int i = 0; i < (int)sceneNames.size(); i++) {
            bool isActive = (sceneNames[i] == active);
            if (isActive) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));

            std::string label = sceneNames[i] + " [" + std::to_string(i + 1) + "]";
            if (ImGui::Button(label.c_str()))
                ctx.sceneManager.setActiveScene(sceneNames[i]);

            if (isActive) ImGui::PopStyleColor();
        }
    }
    ImGui::End();
}

void drawScenePanel(UIContext& ctx) {
    static const std::unordered_map<std::string, void(*)(UIContext&)> scenePanels = {
        { "Cube", drawBenchmarkPanel },
        { "Car",  drawCubeControlsPanel },
    };

    std::string active = ctx.sceneManager.getActiveScene()->getName();
    auto it = scenePanels.find(active);
    if (it != scenePanels.end())
        it->second(ctx);
}

void im_gui(GLFWwindow* window, UIContext& ctx) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    myStyle();

    if (ctx.sceneManager.getSatCount() > 0)
        ctx.averageTimePerSAT = ctx.sceneManager.getSatTime() / ctx.sceneManager.getSatCount();
    else
        ctx.averageTimePerSAT = 0;

    drawScenesPanel(ctx);
    drawPostProcessingPanel(ctx);
    drawScenePanel(ctx);
    drawGridSettingsPanel(ctx);
    drawPerformancePanel(ctx);

    if (autoRun && !benchmarkRunning) {
        filename = std::string("test_") + bmLabel + ".xls";
        ctx.uiConfig.cellSize = grid.cellSize;
        ctx.benchmarkConfig = ctx.uiConfig;

        frozenUseGrid = useGrid;
        benchmarkRunning = true;
        benchmarkStartTime = glfwGetTime();

        benchmarkResult = BenchmarkResult();
        benchmarkResult.config = ctx.benchmarkConfig;

        ctx.sceneManager.resetSatStats();

        if (useTetrahedron)
            ctx.sceneManager.spawnCubesInScene(ctx.uiConfig.objectCount, ctx.uiConfig.randomSeed, ctx.tetraModel);
        else
            ctx.sceneManager.spawnCubesInScene(ctx.uiConfig.objectCount, ctx.uiConfig.randomSeed, ctx.cubeModel);
    }

    processInput(window);
}