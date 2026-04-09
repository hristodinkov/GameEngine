//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#include "SinMovement.h"
#include "GameObject.h"
#include <cmath>

void SinMovement::update(float deltaTime)
{
    localTime += deltaTime;

    float value = sin(localTime * speed + phaseOffset) * amplitude;

    glm::vec3 pos = owner->getPos();

    if (axis == 0) pos.x = value;
    else if (axis == 1) pos.y = value;
    else pos.z = value;

    pos = clampToBorder(pos,border);
    owner->setPos(pos);
}
glm::vec3 SinMovement::clampToBorder(const glm::vec3& pos, float border) {
    glm::vec3 p = pos;
    p.x = glm::clamp(p.x, -border, border);
    p.y = glm::clamp(p.y, -border, border);
    p.z = glm::clamp(p.z, -border, border);
    return p;
}

