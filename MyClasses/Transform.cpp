//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

Transform::Transform(): modelMatrix(glm::mat4(1.0f)) {

}

void Transform::translate(glm::vec3 translation) {
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


