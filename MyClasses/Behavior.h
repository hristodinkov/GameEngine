//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#ifndef RAWENGINE_BEHAVIOUR_H
#define RAWENGINE_BEHAVIOUR_H

#pragma once
#include <memory>

class GameObject;

class Behavior {
public:
    GameObject* owner = nullptr;
    virtual ~Behavior() = default;
    virtual void update(float deltaTime) = 0;
};
#endif //RAWENGINE_BEHAVIOUR_H