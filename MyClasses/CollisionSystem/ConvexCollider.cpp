
//
// Created by Hristo Dinkov on 11.3.2026 г..
//

#include "ConvexCollider.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <glad/glad.h>

ConvexCollider::ConvexCollider(const std::vector<glm::vec3> &vertices, const std::vector<unsigned int>& indices, const glm::mat4 modelMatrix) {
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

void ConvexCollider::drawEdges() const {

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    for (size_t i = 0; i < edgeIndexPairs.size(); i++) {

        unsigned int indexA = edgeIndexPairs[i].first;
        unsigned int indexB = edgeIndexPairs[i].second;

        const glm::vec3& a = worldVertices[indexA];
        const glm::vec3& b = worldVertices[indexB];


        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

    }
    glEnd();
}