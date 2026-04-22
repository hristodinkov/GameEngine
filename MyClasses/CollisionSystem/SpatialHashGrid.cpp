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

void SpatialHashGrid::buildGrid(const std::vector<std::shared_ptr<GameObject>>&objects) {
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







