//
// Created by Hristo Dinkov on 11.3.2026 г..
//

#ifndef RAWENGINE_CONVEXCOLLIDER_H
#define RAWENGINE_CONVEXCOLLIDER_H
#include <map>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <set>
#include "../MyClasses/Shader.h"


class Shader;

class ConvexCollider {
public:
    std::vector<glm::vec3> localVertices;
    std::vector<unsigned int> modelIndices;
    std::vector<glm::vec3> worldVertices;
    std::vector<glm::vec3> faceNormals;
    std::vector<glm::vec3> edges;

    GLuint lineVAO = 0;
    GLuint lineVBO = 0;


    glm::vec3 aabbMin ;
    glm::vec3 aabbMax;
    std::vector<std::pair<unsigned int, unsigned int>> edgeIndexPairs;

    ConvexCollider(const std::vector<glm::vec3>& vertices,const std::vector<unsigned int>& indices,const glm::mat4 modelMatrix);

    void update(const glm::mat4& worldTransform);

    std::vector<glm::vec3> getFaceNormals() const;
    std::vector<glm::vec3> getEdges() const;

    void project(const glm::vec3& axis, float& outMin, float& outMax) const;

    void addEdge(unsigned int a, unsigned int b,std::map<std::pair<unsigned int, unsigned int>, int> &edgeCount);

    std::vector<glm::vec3> getLineVertices() const;


};



#endif //RAWENGINE_CONVEXCOLLIDER_H