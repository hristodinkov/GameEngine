#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"
#include "../MyClasses/Transform.h"

namespace core {
    class Model :public Transform{
    private:
        std::vector<core::Mesh> meshes;
        //glm::mat4 modelMatrix;
    public:
        Model(std::vector<core::Mesh> meshes) : Transform(), meshes(meshes) {}

        void render();


    };
}
