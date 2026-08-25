//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#include "../MyClasses/Utilities/Random.h"


glm::vec2 rand2(const glm::vec2 &co, float offset) {
    float x = glm::fract(sinf(glm::dot(co + glm::vec2(offset), glm::vec2(12.9898f, 78.233f))) * 43758.5453f);
    float y = glm::fract(sinf(glm::dot(glm::vec2(co.y, co.x) + glm::vec2(offset), glm::vec2(43.5453f, 27.231f))) * 28573.9182f);
    return glm::vec2(x, y);
}

glm::vec3 rand3(const glm::vec3 &co, float offset) {
    glm::vec2 r1 = rand2(glm::vec2(co.x,co.y), offset);
    glm::vec2 r2 = rand2(glm::vec2(co.y,co.z), offset);
    return glm::vec3(r1.x, r1.y, r2.x);
}
