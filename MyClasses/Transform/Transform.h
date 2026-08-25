//
// Created by Hristo Dinkov on 13.10.2025 г..
//

#ifndef RAWENGINE_TRANFORMABLE_H
#define RAWENGINE_TRANFORMABLE_H
#include <type_traits>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>



class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scaleFactors;
    glm::mat4 modelMatrix;

    Transform();

    void setPos(const glm::vec3& pos);
    void setRotation(const glm::vec3& rot);
    void setScale(const glm::vec3& scale);

    void translate(const glm::vec3& delta);
    void rotate(const glm::vec3& deltaEuler);
    void scale(const glm::vec3& deltaScale);

    glm::mat4 getModelMatrix() const;
    void updateModelMatrix();

private:


};


#endif //RAWENGINE_TRANFORMABLE_H