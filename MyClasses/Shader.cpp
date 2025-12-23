//
// Created by Hristo Dinkov on 22.12.2025 г..
//

#include "Shader.h"
#include <string>


std::string get_file_contents(const char *filename) {
    std::ifstream fileStream(filename, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filename);
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    //create vertex shader obj and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //attach vertex shader source to the vertex shader obj
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    //compile vertex to machine code
    glCompileShader(vertexShader);

    //creating a fragment shader obj
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //attach fragment shader source to the fragment shader obj
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    //compile fragment to machine code
    glCompileShader(fragmentShader);

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::Activate() {
    glUseProgram(ID);
}
void Shader::Delete() {
    glDeleteProgram(ID);
}

GLint Shader::GetUniformLocation(const char* name) {
    return glGetUniformLocation(ID,name);
}

void Shader::SetVec3Uniform(const char *name, const glm::vec3 &value) {
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}
void Shader::SetMat4Uniform(const char *name, const glm::mat4 &value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetFloatUniform(const char *name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetIntUniform(const char *name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}



