//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#include "GameObject.h"

#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(std::string name) : name(std::move(name)) {}

void GameObject::addChild(std::shared_ptr<GameObject> child) {
    child->parent = this;
    children.push_back(child);
}

std::shared_ptr<GameObject> GameObject::addChild(const GameObject& child) {
    auto newChild = std::make_shared<GameObject>(child);
    newChild->parent = this;
    children.push_back(newChild);
    return newChild;
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
}
glm::mat4 GameObject::getWorldTransform() const {
    if (parent)
        return parent->getWorldTransform() * getModelMatrix();
    return getModelMatrix();
}

void GameObject::render(GLuint shaderProgram,const glm::mat4& projection,const glm::mat4& view) {

    if (model.has_value()) {
        glm::mat4 mvp = projection * view * getWorldTransform();
        GLint mvpLoc = glGetUniformLocation(shaderProgram, "mvpMatrix");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        model->render();
    }

    for (auto& c : children)
        c->render(shaderProgram, projection, view);
}