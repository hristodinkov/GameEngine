//
// Created by Hristo Dinkov on 20.11.2025 г..
//

#include "LightObj.h"

glm::vec3 LightObj::getPos() const {
    return position;
}
glm::vec4 LightObj::getColor() const {
    return color;
}
float LightObj::getRadius() const {
    return radius;
}
