//
// Created by Hristo Dinkov on 14.4.2026 г..
//

#include "SpatialHashGrid.h"
#include "../GameObject.h"


SpatialHashGrid::CellCoord SpatialHashGrid::toCell(const glm::vec3 &pos) const {
    return {
        (int) std::floor(pos.x/cellSize),
        (int) std::floor(pos.y/cellSize),
        (int) std::floor(pos.z/cellSize)
    };
}

void SpatialHashGrid::buildGrid(const std::vector<std::shared_ptr<GameObject> > &objects) {
    table.clear();

    for (auto &obj : objects) {
        CellCoord c = toCell(obj->transform.position);
        table[c].push_back(obj.get());
    }
}

std::vector<std::pair<GameObject *, GameObject *> > SpatialHashGrid::computePairs() {
    std::vector<std::pair<GameObject *, GameObject *>> pairs;

    static const std::vector<CellCoord> neighborOffsets = {
        {0,0,0}, {1,0,0}, {-1,0,0},
        {0,1,0}, {0,-1,0},
        {0,0,1}, {0,0,-1}
    };

    for (auto& [coord, objects] : table) {
        for (auto& off : neighborOffsets) {
            CellCoord neighbor = {coord.i+off.i, coord.j+off.j, coord.k+off.k};

            if (!table.contains(neighbor)) {
                continue;
            }

            auto& others = table[neighbor];

            for (auto* A : objects) {
                for (auto* B : others) {
                    if (A<B) {
                        pairs.emplace_back(A,B);
                    }
                }
            }
        }
    }
    return pairs;
}

std::vector<glm::vec3> SpatialHashGrid::generateAABBLineVertices(const glm::vec3 &minCorner, const glm::vec3 &maxCorner) {
    glm::vec3 v0 = minCorner;
    glm::vec3 v1 = {maxCorner.x, minCorner.y, minCorner.z};
    glm::vec3 v2 = {maxCorner.x, maxCorner.y, minCorner.z};
    glm::vec3 v3 = {minCorner.x, maxCorner.y, minCorner.z};

    glm::vec3 v4 = {minCorner.x, minCorner.y, maxCorner.z};
    glm::vec3 v5 = {maxCorner.x, minCorner.y, maxCorner.z};
    glm::vec3 v6 = maxCorner;
    glm::vec3 v7 = {minCorner.x, maxCorner.y, maxCorner.z};

    return {
        // bottom square
        v0, v1,
        v1, v2,
        v2, v3,
        v3, v0,
        // top square
        v4, v5,
        v5, v6,
        v6, v7,
        v7, v4,
        // vertical edges
        v0, v4,
        v1, v5,
        v2, v6,
        v3, v7
    };
}

void SpatialHashGrid::drawDebug(GLuint debugVAO, GLuint debugVBO, Shader& lineShader)
{
    for (const auto& [coord, objects] : table)
    {
        glm::vec3 minCorner(
            coord.i * cellSize,
            coord.j * cellSize,
            coord.k * cellSize
        );

        glm::vec3 maxCorner = minCorner + glm::vec3(cellSize);

        auto verts = generateAABBLineVertices(minCorner, maxCorner);

        glBindVertexArray(debugVAO);
        glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), verts.data(), GL_DYNAMIC_DRAW);

        lineShader.SetVec3Uniform("color", glm::vec3(0,1,0)); // green grid
        glDrawArrays(GL_LINES, 0, verts.size());
    }
}





