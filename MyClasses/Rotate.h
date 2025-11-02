//
// Created by Hristo Dinkov on 27.10.2025 г..
//

#ifndef RAWENGINE_ROTATE_H
#define RAWENGINE_ROTATE_H

#pragma once
#include "Behavior.h"
#include <glm/vec3.hpp>

class Rotate : public Behavior {
    glm::vec3 axis;
    float speed;
public:
    Rotate(glm::vec3 axis, float speed): axis(axis), speed(speed) {}
    void update(float deltaTime) override;
};


#endif //RAWENGINE_ROTATE_H