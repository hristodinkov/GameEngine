#pragma once

#include <vector>
#include <glad/glad.h>
#include "vertex.h"

namespace core {
    class Mesh {
    private:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
        void render();
        static Mesh generateQuad();
        const std::vector<Vertex>& getVertices() const { return vertices; }
        const std::vector<GLuint>& getIndices() const { return indices; }

    private:
        void setupBuffers();
    };
}