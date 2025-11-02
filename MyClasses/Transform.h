//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#ifndef RAWENGINE_TRANFORMABLE_H
#define RAWENGINE_TRANFORMABLE_H
#include <type_traits>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>



class Transform {
protected:

    glm::mat4 modelMatrix;
    glm::vec3 rotationEuler; // store rotation in radians
    glm::vec3 position;
    glm::vec3 scaleFactors;
public:
    Transform();
    void translate(glm::vec3 translation);
    void rotate(glm::vec3 axis, float radians);
    void scale(glm::vec3 scale);
    glm::mat4 getModelMatrix() const;

    void setRotation(glm::vec3 eulerAngles);
private:
    void updateModelMatrix();
};


#endif //RAWENGINE_TRANFORMABLE_H