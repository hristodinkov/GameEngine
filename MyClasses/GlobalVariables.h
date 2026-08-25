//
// Created by Hristo Dinkov on 24.8.2026 г..
//

#ifndef RAWENGINE_GLOBALVARIABLES_H
#define RAWENGINE_GLOBALVARIABLES_H


#pragma once
#include <string>
#include <vector>
#include "CollisionSystem/Benchmark.h"
#include "CollisionSystem/SpatialHashGrid.h"
#include "UI/Style.h"
#include "Utilities/Camera.h"

extern Style style;
extern Camera camera;
extern SpatialHashGrid grid;
extern int g_width, g_height;

extern bool showFileLockedPopup;
extern std::string lockedFilename;

extern bool benchmarkRunning;
extern double benchmarkStartTime;
extern BenchmarkResult benchmarkResult;
extern bool testIsRunning;
extern std::string filename;
extern bool useGrid;
extern bool frozenUseGrid;
extern bool useTetrahedron;
extern int benchmarkFrameCounter;

extern int bmObjectCount;
extern int bmDuration;
extern bool bmCamera;
extern char bmLabel[64];
extern bool autoRun;

bool isFileLocked(const std::string& filename);
void myStyle();
void processInput(GLFWwindow *window);


#endif //RAWENGINE_GLOBALVARIABLES_H