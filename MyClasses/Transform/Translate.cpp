//
// Created by Hristo Dinkov on 21.11.2025 г..
//

#include "../MyClasses/Transform/Translate.h"

#include "../Utilities/GameObject.h"

void Translate::update(float deltaTime) {
    owner->transform.translate(direction * speed * deltaTime);

}
