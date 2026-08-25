//
// Created by Hristo Dinkov on 25.8.2026 г..
//

#include "GlobalVariables.h"
#include <fstream>
#include <glm/glm.hpp>

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

glm::vec3 guiLightPos = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec4 guiLightColor = glm::vec4(184.0f/256.0f, 23.0f/256.0f, 222.0f/256.0f, 1.0f);
float guiLightRadius = 35.0f;
float guiShininess = 75.0f;
float guiSpecular = 42.0f;
float guiAmbient = 0.25f;

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