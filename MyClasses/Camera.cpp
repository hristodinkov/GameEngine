//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#include "Camera.h"




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

        float sensitivity = 0.002f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        //around world up vector
        this->rotate(glm::vec3(0.0f, 1.0f, 0.0f), (float)xoffset);

        // Compute camera right vector
        glm::vec3 up      = glm::vec3(0.0f, 1.0f, 0.0f); // always world up
        glm::vec3 forward = -glm::normalize(glm::vec3(modelMatrix[2]));
        glm::vec3 right   = glm::normalize(glm::cross(forward, up));

        // Rotate around camera right for pitch
        this->rotate(right, (float)yoffset);


    }
    else {
        rotating = false;
    }

}