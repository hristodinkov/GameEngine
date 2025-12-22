//
// Created by Hristo Dinkov on 22.12.2025 г..
//

#ifndef VBO_ClASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO {
    public:
    GLuint ID;
    VBO(GLfloat* vertices,GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();

};


#endif //RAWENGINE_VBO_H