//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#ifndef RAWENGINE_GAMEOBJECT_H
#define RAWENGINE_GAMEOBJECT_H
#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Transform.h"
#include "Behavior.h"
#include "../core/model.h"


class GameObject:public Transform{
public:
    std::string name;
    std::optional<core::Model> model;
    GameObject* parent = nullptr;
    std::vector<std::shared_ptr<GameObject>> children;
    std::vector<std::shared_ptr<Behavior>> behaviors;

    GameObject(std::string name = "GameObject");

    virtual ~GameObject() = default;

    void addChild(std::shared_ptr<GameObject> child);

    std::shared_ptr<GameObject> addChild(const GameObject& child);

    void addBehavior(std::shared_ptr<Behavior> behavior);

    void update(float deltaTime);

    glm::mat4 getWorldTransform() const;

    virtual void render(GLuint shaderProgram,
                const glm::mat4& projection,
                const glm::mat4& view);
};


#endif //RAWENGINE_GAMEOBJECT_H