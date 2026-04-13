//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#include "GameObject.h"

#include <algorithm>
#include <__msvc_ranges_to.hpp>
#include <glm/gtc/type_ptr.hpp>


GameObject::GameObject(std::string name) : name(std::move(name)) {}

void GameObject::addChild(std::shared_ptr<GameObject> child) {
    child->parent = this;
    children.push_back(child);
}

void GameObject::addBehavior(std::shared_ptr<Behavior> behavior) {
    behaviors.push_back(behavior);
    behavior->owner = this;
}

void GameObject::update(float deltaTime) {

    for (auto& b : behaviors)
        b->update(deltaTime);

    for (auto& c : children)
        c->update(deltaTime);

    if (collider)
        collider->update(getWorldTransform());

}

void GameObject::render(Shader& shader,const glm::mat4& projection,const glm::mat4& view) {

    if (!model.has_value())
        return;

    glm::vec3 color ;
    if (isColliding) {
        color = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else {
        color = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    glm::mat4 modelMat = getWorldTransform();
    glm::mat4 mvp = projection * view * modelMat;

    shader.SetVec3Uniform("color", color);
    shader.SetMat4Uniform("modelMatrix", modelMat);
    shader.SetMat4Uniform("viewMatrix", view);
    shader.SetMat4Uniform("projMatrix", projection);
    shader.SetMat4Uniform("mvpMatrix", mvp);

    model->render();

    for (auto& c : children)
        c->render(shader, projection, view);
}

glm::mat4 GameObject::getWorldTransform() const {
    if (parent)
        return parent->getWorldTransform() * transform.getModelMatrix();
    return transform.getModelMatrix();
}

void GameObject::setPos(const glm::vec3& p) {
    transform.setPos(p);
}

glm::vec3 GameObject::getPos() const {
    return transform.position;
}
