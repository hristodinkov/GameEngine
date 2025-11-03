//
// Created by Hristo Dinkov on 3.11.2025 г..
//

#include "SceneManager.h"

#include <iostream>

std::shared_ptr<Scene> SceneManager::createScene(const std::string& name) {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scenesDictionary[name] = scene;
    if (!currentScene) {
        currentScene = scene;
    }
    return scene;
}

void SceneManager::setActiveScene(const std::string& name) {
    auto sceneToActivate = scenesDictionary.find(name); //scenes.end() is a special iterator that points one past the last valid element in the map
    if (sceneToActivate != scenesDictionary.end()) {
        currentScene = sceneToActivate->second;
        std::cout << "Switched to scene: " << name << std::endl;
    } else {
        std::cerr << "Scene not found: " << name << std::endl;
    }
}

std::shared_ptr<Scene> SceneManager::getActiveScene() const {
    return currentScene;
}

void SceneManager::update(float deltaTime) {
    if (currentScene)
        currentScene->update(deltaTime);
}

void SceneManager::render(GLuint shaderProgram,
                          const glm::mat4& projection,
                          const glm::mat4& view) {
    if (currentScene)
        currentScene->render(shaderProgram, projection, view);
}