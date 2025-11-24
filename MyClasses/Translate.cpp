//
// Created by Hristo Dinkov on 21.11.2025 г..
//

#include "Translate.h"

#include "GameObject.h"

void Translate::update(float deltaTime) {
    owner->translate(direction * speed * deltaTime);
}
