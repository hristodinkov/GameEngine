//
// Created by Hristo Dinkov on 20.11.2025 г..
//

#ifndef RAWENGINE_LIGHTOBJ_H
#define RAWENGINE_LIGHTOBJ_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


class LightObj {
private:
    glm::vec3 position;
    glm::vec4 color;
    float radius;
    int type;
public:
    LightObj(glm::vec3 pos,glm::vec4 color,float radius,int type = 1):position(pos),color(color), radius(radius) {}
    ~LightObj() = default;

    LightObj& operator = (const LightObj& anotherLight) {
        if (this != &anotherLight) {
            this->position = anotherLight.getPos();
            this->color = anotherLight.getColor();
        }
        return *this;
    }

    glm::vec3 getPos() const;
    glm::vec4 getColor() const;
    float getRadius() const;
};
#endif //RAWENGINE_LIGHTOBJ_H