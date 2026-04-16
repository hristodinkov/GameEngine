//
// Created by Hristo Dinkov on 3.11.2025 г..
//

#ifndef RAWENGINE_SCENEMANAGER_H
#define RAWENGINE_SCENEMANAGER_H

#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Scene.h"
class SceneManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenesDictionary;
    std::shared_ptr<Scene> currentScene = nullptr;
    core::Model model;
    int satTestsThisFrame;
    double satTimeThisFrame;
public:

    SceneManager(core::Model);
    std::shared_ptr<Scene> createScene(const std::string& name);
    void setActiveScene(const std::string& name);
    std::shared_ptr<Scene> getActiveScene() const;

    int getSatCount() const;
    double getSatTime() const;
    void resetSatStats();

    void spawnCubesInScene(int count,int seed);
    void clearObjects();
    std::vector<std::pair<GameObject*,GameObject*>> computeBruteForcePairs();
    void runSAT(GameObject* A,GameObject*B);

    void update(float deltaTime);
    void render(Shader& shader,const glm::mat4& projection,const glm::mat4& view);
};


#endif //RAWENGINE_SCENEMANAGER_H