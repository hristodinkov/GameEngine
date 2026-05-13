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
    for (const glm::vec3& normal : normalsA) {
        if (glm::length(normal) > 0.0001f) {
            axes.push_back(glm::normalize(normal));
        }
    }

    const auto& normalsB = B.getFaceNormals();
    for (const glm::vec3& normal : normalsB) {
        if (glm::length(normal) > 0.0001f) {
            axes.push_back(glm::normalize(normal));
        }
    }

    const auto& edgesA = A.getEdges();
    const auto& edgesB = B.getEdges();

    for (const auto& eA : edgesA)
    {
        glm::vec3 a0 = A.worldVertices[eA.first];
        glm::vec3 a1 = A.worldVertices[eA.second];
        glm::vec3 edgeA = a1 - a0;

        for (const auto& eB : edgesB)
        {
            glm::vec3 b0 = B.worldVertices[eB.first];
            glm::vec3 b1 = B.worldVertices[eB.second];
            glm::vec3 edgeB = b1 - b0;

            glm::vec3 axis = glm::cross(edgeA, edgeB);

            if (glm::length(axis) > 0.0001f)
            {
                axes.push_back(glm::normalize(axis));
            }
        }
    }

    for (const auto& axis : axes) {
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



