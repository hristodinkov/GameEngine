//
// Created by Hristo Dinkov on 27.10.2025 г..
//

#include "Rotate.h"

#include "../Utilities/GameObject.h"


void Rotate::update(float deltaTime) {
    owner->transform.rotate(axis*speed*deltaTime);


}
