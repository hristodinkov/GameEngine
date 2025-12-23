//
// Created by Hristo Dinkov on 22.12.2025 г..
//

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H
#include <glad/glad.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);


class Shader {
public:
    GLuint ID;
    Shader(const char*vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();

    GLint GetUniformLocation(const char* name);
    void SetVec3Uniform(const char* name, const glm::vec3& value);
    void SetFloatUniform(const char* name,float value);
    void SetMat4Uniform(const char* name, const glm::mat4& mat);
    void SetIntUniform(const char* name, int value);

};


#endif //RAWENGINE_SHADER_H