//
// Created by Hristo Dinkov on 14.4.2026 г..
//

#ifndef RAWENGINE_SPATIALHASHGRID_H
#define RAWENGINE_SPATIALHASHGRID_H
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>

using GLuint = unsigned int;

class Shader;
class GameObject;

class SpatialHashGrid {
public:
    float cellSize = 5.0f;

    void buildGrid(const std::vector<std::shared_ptr<GameObject>>& objects);

    std::vector<std::pair<GameObject*, GameObject*>> computePairs();


    private:
    struct CellCoord {
        int i, j, k;
        bool operator==(const CellCoord& other) const {
            return i == other.i && j == other.j && k == other.k;
        }
    };

    struct CellCoordHash {
        size_t operator()(const CellCoord& c) const {
            return (size_t)(
                //The numbers here a based on the paper Optimized Spatial Hashing for Collision Detection of Deformable Objects by
                //Teschner et al. (2003) https://cgl.ethz.ch/Downloads/Publications/Papers/2003/Tes03/Tes03.pdf
                c.i * 73856093 ^
                c.j * 19349663 ^
                c.k * 83492791
            );
        }
    };

    std::unordered_map<CellCoord, std::vector<GameObject*>, CellCoordHash> table;

    CellCoord toCell(const glm::vec3& pos) const;
};


#endif //RAWENGINE_SPATIALHASHGRID_H