//
// Created by Hristo Dinkov on 21.11.2025 г..
//

#ifndef RAWENGINE_TRANSLATE_H
#define RAWENGINE_TRANSLATE_H
#include <glm/vec3.hpp>

#include "Behavior.h"


class Translate:public Behavior {
    float speed;
    glm::vec3 direction;
    public:
    Translate(float speed, glm::vec3 direction): speed(speed), direction(direction) {}
    void update(float deltaTime) override;
};


#endif //RAWENGINE_TRANSLATE_H