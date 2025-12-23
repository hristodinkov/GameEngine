//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#ifndef RAWENGINE_SCENE_H
#define RAWENGINE_SCENE_H

#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"

class Scene {
public:
    std::vector<std::shared_ptr<GameObject>> objects;

    std::shared_ptr<GameObject> addObject(std::shared_ptr<GameObject> obj,...);
    std::shared_ptr<GameObject> addObject(const GameObject& obj,...);
    void update(float deltaTime);

    void render(Shader& shader,const glm::mat4& projection,const glm::mat4& view);

};

#endif //RAWENGINE_SCENE_H