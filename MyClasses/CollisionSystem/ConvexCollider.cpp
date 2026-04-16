
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

    std::map<std::pair<unsigned int, unsigned int>, int> edgeCount;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        addEdge(i0, i1, edgeCount);
        addEdge(i1, i2, edgeCount);
        addEdge(i2, i0, edgeCount);

        glm::vec3 v0 = vertices[i0];
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];
        faceNormals.push_back(glm::normalize(glm::cross(v1 - v0, v2 - v0)));
    }

    edgeIndexPairs.clear();

    for (auto &entry : edgeCount)
    {
        const auto &edge = entry.first;
        int count = entry.second;

        if (count == 1)  // boundary edge
        {
            edgeIndexPairs.push_back(edge);
        }
    }
    allEdges.clear();

    for (size_t i = 0; i < modelIndices.size(); i += 3)
    {
        unsigned int i0 = modelIndices[i];
        unsigned int i1 = modelIndices[i + 1];
        unsigned int i2 = modelIndices[i + 2];

        allEdges.push_back({i0, i1});
        allEdges.push_back({i1, i2});
        allEdges.push_back({i2, i0});
    }

};

void ConvexCollider::update(const glm::mat4 &worldTransform) {

    for (size_t i = 0; i < localVertices.size(); i ++) {
        glm::vec4 world = worldTransform * glm::vec4(localVertices[i], 1.0f);
        worldVertices[i] = glm::vec3(world);
    }
}

std::vector<glm::vec3> ConvexCollider::getFaceNormals() const {
    return faceNormals;
}

std::vector<std::pair<unsigned int, unsigned int>> ConvexCollider::getEdges() const {
    return allEdges;
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

void ConvexCollider::addEdge(unsigned int a, unsigned int b, std::map<std::pair<unsigned int, unsigned int>, int> &edgeCount) {
    if (a > b) {
        std::swap(a, b);
    }
    edgeCount[{a, b}]++;
}


