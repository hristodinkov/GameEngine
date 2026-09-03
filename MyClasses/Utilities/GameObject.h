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

#include "../Transform/Transform.h"
#include "Behavior.h"

#include "../Transform/Translate.h"
#include "../../core/model.h"
#include "../CollisionSystem/ConvexCollider.h"
#include "../../core/texture.h"


class GameObject {
public:
    std::string name;
    std::optional<core::Model> model;
    std::optional<core::Texture> texture;
    GameObject* parent = nullptr;
    std::vector<std::shared_ptr<GameObject>> children;
    std::vector<std::shared_ptr<Behavior>> behaviors;
    std::shared_ptr<ConvexCollider> collider;
    bool isColliding = false;
    Transform transform = Transform();

    GameObject(std::string name = "GameObject");

    virtual ~GameObject() = default;

    void addChild(std::shared_ptr<GameObject> child);

    void addBehavior(std::shared_ptr<Behavior> behavior);

    void update(float deltaTime);

    glm::mat4 getWorldTransform() const;

    void render(Shader& shader,const glm::mat4& projection,const glm::mat4& view);

    void setPos(const glm::vec3& pos);

    glm::vec3 getPos() const;

};


#endif //RAWENGINE_GAMEOBJECT_H