//
// Created by Hristo Dinkov on 25.8.2026 г..
//

#include "GlobalVariables.h"
#include <fstream>

// Core engine state
Style style;
Camera camera;
SpatialHashGrid grid;
int g_width = 1200;
int g_height = 800;

// Excel/file popup state
bool showFileLockedPopup = false;
std::string lockedFilename;

// Benchmark state
bool benchmarkRunning = false;
double benchmarkStartTime = 0.0;
BenchmarkResult benchmarkResult;
bool testIsRunning = false;
std::string filename;
bool useGrid = false;
bool frozenUseGrid = false;
bool useTetrahedron = false;
int benchmarkFrameCounter = 0;

int bmObjectCount = 10;
int bmDuration = 200;
bool bmCamera = true;
char bmLabel[64] = "baseline";
bool autoRun = false;

bool isFileLocked(const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    return !file.is_open();
}

void myStyle() {
    style.SetupImGuiStyle();
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.Move(window);
    camera.Rotate(window);
}