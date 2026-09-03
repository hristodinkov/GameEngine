//
// Created by Hristo Dinkov on 24.8.2026 г..
//

#ifndef RAWENGINE_UIPANELS_H
#define RAWENGINE_UIPANELS_H


#pragma once
#include <GLFW/glfw3.h>
#include "UIContext.h"

void drawCubeControlsPanel(UIContext& ctx);
void drawBenchmarkPanel(UIContext& ctx);
void drawPerformancePanel(UIContext& ctx);
void drawGridSettingsPanel(UIContext& ctx);
void drawPostProcessingPanel(UIContext& ctx);
void drawScenesPanel(UIContext& ctx);
void drawScenePanel(UIContext& ctx);
void drawCollisionPanels(UIContext& ctx);
void drawLightingPanel(UIContext& ctx);
void im_gui(GLFWwindow* window, UIContext& ctx);

#endif