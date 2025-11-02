//
// Created by Hristo Dinkov on 27.10.2025 г..
//

#include "Rotate.h"

#include "GameObject.h"


void Rotate::update(float deltaTime) {
    owner->rotate(axis, speed * deltaTime);
}
