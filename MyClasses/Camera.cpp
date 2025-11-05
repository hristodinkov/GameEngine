//
// Created by Hristo Dinkov on 13.10.2025 г..
//
#pragma once

#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
//     glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//     glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//     glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//     glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//     glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

glm::vec3 Camera::getCameraPos() const {
    return this->cameraPos;
}
void Camera::Move(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->translate(glm::vec3(0,0,-speed));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->translate(glm::vec3(0,0,speed));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->translate(glm::vec3(-speed,0,0));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->translate(glm::vec3(speed,0,0));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        this->translate(glm::vec3(0,-speed,0));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        this->translate(glm::vec3(0,speed,0));
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        fov -= fovSpeed * 0.01f; // decrease FOV to zoom in
        if (fov < 1.0f) fov = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        fov += fovSpeed * 0.01f; // increase FOV to zoom out
        if (fov > 120.0f) fov = 120.0f;
    }
}
void Camera::Rotate(GLFWwindow *window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!rotating) {
            rotating = true;
            lastX = xpos;
            lastY = ypos;
            return;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.02f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // 1. Актуализираме ъглите (тези са член-променливи на Camera)
        // yRotation е Yaw (ляво/дясно), управлява се от X на мишката
        yRotation += (float)xoffset;

        // xRotation е Pitch (горе/долу), управлява се от Y на мишката
        xRotation += (float)yoffset;

        // 2. Ограничаваме ъгъла (Pitch), за да избегнем "преобръщане"
        // Това е най-важната част!
        if (xRotation > 89.0f) {
            xRotation = 89.0f;
        }
        if (xRotation < -89.0f) {
            xRotation = -89.0f;
        }

        // 3. Запазваме настоящата позиция на камерата
        // (Твоята функция Move() се грижи за тази стойност)
        glm::vec3 currentPosition = glm::vec3(modelMatrix[3]);

        // 4. Изчисляваме НОВА, ЧИСТА матрица на ротация от ъглите

        // Първо въртене около световната Y-ос (Yaw)
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        // След това въртене около локалната X-ос (Pitch)
        // Забележи, че я прилагаме към *вече* завъртяната yaw матрица
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));

        // 5. Задаваме новата 'modelMatrix' да бъде само ротацията...
        modelMatrix = rotationMatrix;

        // 6. ... и след това връщаме позицията на мястото й.
        modelMatrix[3] = glm::vec4(currentPosition, 1.0f);
    }
    else {
        rotating = false;
    }

}