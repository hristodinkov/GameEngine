//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

Transform::Transform()
    : position(0.0f), rotation(0.0f), scaleFactors(1.0f)
{
    updateModelMatrix();
}

void Transform::setPos(const glm::vec3& pos) {
    position = pos;
    updateModelMatrix();
}

void Transform::setRotation(const glm::vec3& rot) {
    rotation = rot;
    updateModelMatrix();
}

void Transform::setScale(const glm::vec3& scale) {
    scaleFactors = scale;
    updateModelMatrix();
}

void Transform::translate(const glm::vec3& delta) {
    position += delta;
    updateModelMatrix();
}

void Transform::rotate(const glm::vec3& deltaEuler) {
    rotation += deltaEuler;
    updateModelMatrix();
}

void Transform::scale(const glm::vec3& deltaScale) {
    scaleFactors *= deltaScale;
    updateModelMatrix();
}

glm::mat4 Transform::getModelMatrix() const {
    return modelMatrix;
}

void Transform::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);

    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0,0,1));

    modelMatrix = glm::scale(modelMatrix, scaleFactors);
}

