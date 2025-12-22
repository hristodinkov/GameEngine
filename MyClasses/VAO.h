//
// Created by Hristo Dinkov on 22.12.2025 г..
//

#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    GLuint ID;
    VAO();

    void LinkVBO(VBO VBO,GLuint layout);
    void Bind();
    void Unbind();
    void Delete();

};


#endif //RAWENGINE_VAO_H