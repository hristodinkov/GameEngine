//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#ifndef RAWENGINE_SCENE_H
#define RAWENGINE_SCENE_H

#pragma once
#include <vector>
#include <memory>
#include "../Utilities/GameObject.h"
#include <chrono>
#include <string>

class Scene {
private:
    std::string name;

public:
    std::vector<std::shared_ptr<GameObject>> objects;

    Scene(std::string sceneName) : name(std::move(sceneName)) {}

    const std::string& getName() const;

    std::shared_ptr<GameObject> addObject(std::shared_ptr<GameObject> obj);

    void update(float deltaTime);

    void render(Shader& shader,const glm::mat4& projection,const glm::mat4& view);
};

#endif //RAWENGINE_SCENE_H