//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

Transform::Transform(): modelMatrix(glm::mat4(1.0f)) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Transform::translate(glm::vec3 translation) {
    position += translation;
    modelMatrix = glm::translate(modelMatrix, translation);
}

void Transform::rotate(glm::vec3 axis, float radians) {
    modelMatrix = glm::rotate(modelMatrix, radians, axis);
}

void Transform::scale(glm::vec3 scale) {
    modelMatrix = glm::scale(modelMatrix, scale);
}

glm::mat4 Transform::getModelMatrix() const {

    return this->modelMatrix;
}

glm::vec3 Transform::getPos() const {
    return modelMatrix * glm::vec4(position.x,position.y,position.z,1); // TODO: Look at shader programming :-)
    return this->position; // TODO: DOn't keep track of redundant data
}

void Transform::setPos(glm::vec3 pos) {
    this->position = pos;
    this->modelMatrix[3] = glm::vec4(pos, 1.0f);
}

