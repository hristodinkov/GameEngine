//
// Created by Hristo Dinkov on 18.4.2026 г..
//

#include "ColliderModels.h"
#include "../core/mesh.h"
#include "../core/model.h"

#include "ColliderModels.h"

std::vector<Vertex> cubeVertices = {
    // Front (-Z)
    {{-1,-1,-1}, {0,0,-1}, {0,0}},
    {{ 1,-1,-1}, {0,0,-1}, {1,0}},
    {{ 1, 1,-1}, {0,0,-1}, {1,1}},
    {{-1, 1,-1}, {0,0,-1}, {0,1}},

    // Right (+X)
    {{ 1,-1,-1}, {1,0,0}, {0,0}},
    {{ 1,-1, 1}, {1,0,0}, {1,0}},
    {{ 1, 1, 1}, {1,0,0}, {1,1}},
    {{ 1, 1,-1}, {1,0,0}, {0,1}},

    // Back (+Z)
    {{ 1,-1, 1}, {0,0,1}, {0,0}},
    {{-1,-1, 1}, {0,0,1}, {1,0}},
    {{-1, 1, 1}, {0,0,1}, {1,1}},
    {{ 1, 1, 1}, {0,0,1}, {0,1}},

    // Left (-X)
    {{-1,-1, 1}, {-1,0,0}, {0,0}},
    {{-1,-1,-1}, {-1,0,0}, {1,0}},
    {{-1, 1,-1}, {-1,0,0}, {1,1}},
    {{-1, 1, 1}, {-1,0,0}, {0,1}},

    // Top (+Y)
    {{-1, 1,-1}, {0,1,0}, {0,0}},
    {{ 1, 1,-1}, {0,1,0}, {1,0}},
    {{ 1, 1, 1}, {0,1,0}, {1,1}},
    {{-1, 1, 1}, {0,1,0}, {0,1}},

    // Bottom (-Y)
    {{-1,-1, 1}, {0,-1,0}, {0,0}},
    {{ 1,-1, 1}, {0,-1,0}, {1,0}},
    {{ 1,-1,-1}, {0,-1,0}, {1,1}},
    {{-1,-1,-1}, {0,-1,0}, {0,1}},
};

std::vector<GLuint> cubeIndices = {
    0,1,2, 2,3,0,       // front
    4,5,6, 6,7,4,       // right
    8,9,10, 10,11,8,    // back
    12,13,14, 14,15,12, // left
    16,17,18, 18,19,16, // top
    20,21,22, 22,23,20  // bottom
};

std::vector<Vertex> tetraVerts = {
    // Base face
    Vertex({-1, 0, -1}, glm::vec3(0, -1, 0), {0,0}),
    Vertex({ 1, 0, -1}, glm::vec3(0, -1, 0), {1,0}),
    Vertex({ 0, 0,  1}, glm::vec3(0, -1, 0), {0.5f,1}),

    // Side face 1
    Vertex({-1, 0, -1}, glm::normalize(glm::vec3( 2, 1.5,  0)), {0,0}),
    Vertex({ 1, 0, -1}, glm::normalize(glm::vec3( 2, 1.5,  0)), {1,0}),
    Vertex({ 0, 1.5, 0}, glm::normalize(glm::vec3( 2, 1.5,  0)), {0.5f,1}),

    // Side face 2
    Vertex({ 1, 0, -1}, glm::normalize(glm::vec3(-2, 1.5,  2)), {0,0}),
    Vertex({ 0, 0,  1}, glm::normalize(glm::vec3(-2, 1.5,  2)), {1,0}),
    Vertex({ 0, 1.5, 0}, glm::normalize(glm::vec3(-2, 1.5,  2)), {0.5f,1}),

    // Side face 3
    Vertex({ 0, 0,  1}, glm::normalize(glm::vec3( 1, 1.5, -2)), {0,0}),
    Vertex({-1, 0, -1}, glm::normalize(glm::vec3( 1, 1.5, -2)), {1,0}),
    Vertex({ 0, 1.5, 0}, glm::normalize(glm::vec3( 1, 1.5, -2)), {0.5f,1}),
};
std::vector<unsigned int> tetraIndices = {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9,10,11
};


