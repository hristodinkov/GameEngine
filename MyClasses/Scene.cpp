//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#include "Scene.h"

std::shared_ptr<GameObject> Scene::addObject(std::shared_ptr<GameObject> obj,...) {
    objects.push_back(obj);
    return obj;
}
std::shared_ptr<GameObject>Scene::addObject(const GameObject& obj,...) {
    auto newObject = std::make_shared<GameObject>(obj);
    objects.push_back(newObject);
    return newObject;
}

void Scene::update(float deltaTime) {
    for (auto& obj : objects)
        obj->update(deltaTime);
}

void Scene::render(GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view) {
    for (auto& obj : objects)
        obj->render(shaderProgram, projection, view);
}


