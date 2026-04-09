//
// Created by Hristo Dinkov on 7.4.2026 г..
//

#ifndef RAWENGINE_SINMOVEMENT_H
#define RAWENGINE_SINMOVEMENT_H
#include <glm/vec3.hpp>

#include "Behavior.h"

extern float gBenchmarkTime;

class SinMovement:public Behavior{
    float localTime = 0.0f;
    int axis;
    float speed;
    float amplitude;
    float phaseOffset;
    float border = 0;
public:
    SinMovement(int axis,float speed, float amplitude, float phaseOffset,float border = 0)
    :axis(axis), speed(speed), amplitude(amplitude), phaseOffset(phaseOffset),border(border){}

    glm::vec3 clampToBorder(const glm::vec3& pos, float border);
    void update(float deltaTime) override;
};


#endif //RAWENGINE_SINMOVEMENT_H