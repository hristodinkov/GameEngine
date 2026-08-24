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
        { 0, 0, 0},
        { 1, 0, 0},
        {-1, 1, 0}, { 0, 1, 0}, { 1, 1, 0},
        {-1,-1, 1}, { 0,-1, 1}, { 1,-1, 1},
        {-1, 0, 1}, { 0, 0, 1}, { 1, 0, 1},
        {-1, 1, 1}, { 0, 1, 1}, { 1, 1, 1},
    };

    for (auto& pair : table) {
        CellCoord cellCoord = pair.first;
        auto& objectsInCell = pair.second;

        for (auto& offset : neighborOffsets) {
            CellCoord neighbourCoord;
            neighbourCoord.i = cellCoord.i + offset.i;
            neighbourCoord.j = cellCoord.j + offset.j;
            neighbourCoord.k = cellCoord.k + offset.k;

            if (!table.contains(neighbourCoord)) {
                continue;
            }

            auto& objectsInNeighbour = table[neighbourCoord];

            for (auto* objectA : objectsInCell) {
                for (auto* objectB : objectsInNeighbour) {
                    if (objectA < objectB) {
                        pairs.emplace_back(objectA, objectB);
                    }
                }
            }
        }
    }
    return pairs;
}








