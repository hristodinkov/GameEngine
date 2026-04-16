//
// Created by Hristo Dinkov on 11.3.2026 г..
//

#include "Collision.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

#include "ConvexCollider.h"


#pragma once

bool Collision::SATCollision(const ConvexCollider& A, const ConvexCollider& B)
{
    std::vector<glm::vec3> axes;

    const auto& normalsA = A.getFaceNormals();
    for (size_t i = 0; i < normalsA.size(); i++) {
        const glm::vec3& n = normalsA[i];
        if (glm::length(n) > 0.0001f) {
            axes.push_back(glm::normalize(n));
        }

    }

    const auto& normalsB = B.getFaceNormals();
    for (size_t i = 0; i < normalsB.size(); i++) {
        const glm::vec3& n = normalsB[i];
        if (glm::length(n) > 0.0001f) {
            axes.push_back(glm::normalize(n));
        }
    }

    const auto& edgesA = A.getEdges();
    const auto& edgesB = B.getEdges();

    for (size_t i = 0; i < edgesA.size(); i++)
    {

        glm::vec3 a0 = A.worldVertices[edgesA[i].first];
        glm::vec3 a1 = A.worldVertices[edgesA[i].second];
        glm::vec3 edgeA = a1 - a0;

        for (size_t j = 0; j < edgesB.size(); j++)
        {
            glm::vec3 b0 = B.worldVertices[edgesB[j].first];
            glm::vec3 b1 = B.worldVertices[edgesB[j].second];
            glm::vec3 edgeB = b1 - b0;

            glm::vec3 axis = glm::cross(edgeA, edgeB);

            if (glm::length(axis) > 0.0001f)
            {
                axes.push_back(glm::normalize(axis));
            }
        }
    }

    for (size_t i = 0; i < axes.size(); i++) {

        const glm::vec3& axis = axes[i];

        float minA, maxA;
        float minB, maxB;

        A.project(axis, minA, maxA);
        B.project(axis, minB, maxB);

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }
    return true;
}



