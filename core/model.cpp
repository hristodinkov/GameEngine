#include "model.h"
#include <glm/gtc/matrix_transform.hpp>

namespace core {
    void Model::render() {
        
        for (int i = 0; i < meshes.size(); ++i) {
            meshes[i].render();
        }
    }

    std::vector<glm::vec3> Model::getAllVertices() const {
        std::vector<glm::vec3> result;

        for (const auto& mesh : meshes) {
            for (const auto& v : mesh.getVertices()) {
                result.push_back(v.position); // adjust if your struct uses a different name
            }
        }

        return result;
    }

    std::vector<unsigned int> Model::getAllIndices() const {
        std::vector<unsigned int> result;
        unsigned int offset = 0;

        for (const auto& mesh : meshes) {
            for (auto idx : mesh.getIndices()) {
                result.push_back(idx + offset);
            }
            offset += mesh.getVertices().size();
        }

        return result;
    }



}