//
// Created by Hristo Dinkov on 13.10.2025 г..
//
#pragma once

#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void Camera::Move(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->transform.translate(glm::vec3(0,0,-speed));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->transform.translate(glm::vec3(0,0,speed));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->transform.translate(glm::vec3(-speed,0,0));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->transform.translate(glm::vec3(speed,0,0));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        this->transform.translate(glm::vec3(0,-speed,0));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        this->transform.translate(glm::vec3(0,speed,0));
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

        //scale down the movement
        float sensitivity = 0.02f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        //left/right
        yRotation -= (float)xoffset;
        //up/down
        xRotation += (float)yoffset;

        //prevents from flipping upside down
        if (xRotation > 89.0f) {
            xRotation = 89.0f;
        }
        if (xRotation < -89.0f) {
            xRotation = -89.0f;
        }


        transform.rotation = glm::vec3(
            glm::radians(xRotation),
            glm::radians(yRotation),
            0.0f
        );


        transform.updateModelMatrix();

    }
    else {
        rotating = false;
    }
}
glm::vec3 Camera::getPos() const {
    return  transform.position;
}

glm::vec3 Camera::getForward() const {

    float yawRad   = glm::radians(yRotation);
    float pitchRad = glm::radians(xRotation);

    return glm::normalize(glm::vec3(
        cos(pitchRad) * sin(yawRad),
        sin(pitchRad),
        -cos(pitchRad) * cos(yawRad)
    ));
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(getForward(), glm::vec3(0,1,0)));
}

glm::vec3 Camera::getUp() const {
    return glm::normalize(glm::cross(getRight(), getForward()));
}
