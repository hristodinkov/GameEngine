
//
// Created by Hristo Dinkov on 11.3.2026 г..
//

#include "ConvexCollider.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <glad/glad.h>



ConvexCollider::ConvexCollider(const std::vector<glm::vec3> &vertices, const std::vector<unsigned int>& indices, const glm::mat4 modelMatrix) {

    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    // each vertex = vec3 position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);


    localVertices = vertices;
    modelIndices = indices;
    for (auto element: vertices) {
        worldVertices.push_back(glm::vec3(glm::vec4(element,1)*modelMatrix));
    }
    std::set<std::pair<unsigned int, unsigned int>> uniqueEdges;

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        addEdge(i0, i1, uniqueEdges);
        addEdge(i1, i2, uniqueEdges);
        addEdge(i2, i0, uniqueEdges);

        glm::vec3 v0 = vertices[i0];
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];
        faceNormals.push_back(glm::normalize(glm::cross(v1 - v0, v2 - v0)));
    }

    edgeIndexPairs.assign(uniqueEdges.begin(), uniqueEdges.end());
};

void ConvexCollider::update(const glm::mat4 &worldTransform) {
    aabbMin = glm::vec3(FLT_MAX);
    aabbMax = glm::vec3(-FLT_MAX);
    for (size_t i = 0; i < localVertices.size(); i ++) {
        glm::vec4 world = worldTransform * glm::vec4(localVertices[i], 1.0f);
        worldVertices[i] = glm::vec3(world);

        aabbMin = glm::min(aabbMin, worldVertices[i]);
        aabbMax = glm::max(aabbMax, worldVertices[i]);
    }
}

std::vector<glm::vec3> ConvexCollider::getFaceNormals() const {
    return faceNormals;
}

std::vector<glm::vec3> ConvexCollider::getEdges() const {
    return edges;
}
std::vector<glm::vec3> ConvexCollider::getLineVertices() const {
    std::vector<glm::vec3> verts;
    verts.reserve(edgeIndexPairs.size() * 2);

    for (auto& e : edgeIndexPairs) {
        verts.push_back(worldVertices[e.first]);
        verts.push_back(worldVertices[e.second]);
    }

    return verts;
}


void ConvexCollider::project(const glm::vec3& axis, float& outMin, float& outMax) const
{
    float first = glm::dot(worldVertices[0], axis);
    outMin = first;
    outMax = first;

    for (size_t i = 1; i < worldVertices.size(); i++) {
        float p = glm::dot(worldVertices[i], axis);
        outMin = std::min(outMin, p);
        outMax = std::max(outMax, p);
    }
}

void ConvexCollider::addEdge(unsigned int a, unsigned int b, std::set<std::pair<unsigned int, unsigned int>> &uniqueEdges) {
    if (a > b) {
        std::swap(a, b);
    }
    uniqueEdges.insert({a, b});
}

void ConvexCollider::drawEdges(Shader& shader,const glm::mat4& view, const glm::mat4& proj) const {

    std::vector<glm::vec3> lineVerts;
    lineVerts.reserve(edgeIndexPairs.size() * 2);

    for (auto& e : edgeIndexPairs)
    {
        lineVerts.push_back(worldVertices[e.first]);
        lineVerts.push_back(worldVertices[e.second]);
    }

    shader.Activate();
    shader.SetMat4Uniform("viewMatrix", view);
    shader.SetMat4Uniform("projMatrix", proj);
    shader.SetVec3Uniform("color", glm::vec3(0,1,0));

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 lineVerts.size() * sizeof(glm::vec3),
                 lineVerts.data(),
                 GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, lineVerts.size());
    glBindVertexArray(0);

}