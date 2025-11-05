//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#ifndef RAWENGINE_CAMERA_H
#define RAWENGINE_CAMERA_H
#include <glm/vec3.hpp>

#include "Transform.h"
#include <GLFW/glfw3.h>
using namespace std;



class Camera : public Transform {

public:
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraDirection;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 cameraForward;
    float speed;


    bool rotating = false;
    double lastX = 0.0;
    double lastY = 0.0;
    float yRotation = 0.0f;
    float xRotation = 0.0f;
    float sensitivity = 0.1f;

    float fov =45.0f;
    float fovSpeed =5;
public:
    glm::vec3 getCameraPos() const;
    void Move(GLFWwindow *window);
    void Rotate(GLFWwindow *window);

};


#endif //RAWENGINE_CAMERA_H