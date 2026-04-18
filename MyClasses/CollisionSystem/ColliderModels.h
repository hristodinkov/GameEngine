#pragma once
#include "../core/model.h"

extern std::vector<Vertex> cubeVertices ;

extern std::vector<GLuint> cubeIndices ;

extern std::vector<Vertex> tetraVerts ;


// std::vector<glm::vec3> tetraVerts = {
//     { 1,  1,  1},
//     {-1, -1,  1},
//     {-1,  1, -1},
//     { 1, -1, -1}
// };
//
// std::vector<unsigned int> tetraIndices = {
//     0,1,2,
//     0,1,3,
//     0,2,3,
//     1,2,3
// };



extern std::vector<unsigned int> tetraIndices ;
