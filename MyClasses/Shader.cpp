//
// Created by Hristo Dinkov on 22.12.2025 г..
//

#include "Shader.h"

std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw(errno);
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

