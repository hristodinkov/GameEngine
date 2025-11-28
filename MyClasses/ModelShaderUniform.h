//
// Created by Hristo Dinkov on 23.11.2025 г..
//


struct ModelShaderUniforms {
    GLint modelMatrix;
    GLint viewMatrix;
    GLint projMatrix;

    GLint lightPos;
    GLint cameraPos;
    GLint lightRadius;
    GLint ambientStrength;
    GLint specularStrength;
    GLint shininess;

    GLint texture0;
    GLint lightColor;
};
#ifndef RAWENGINE_MODELSHADERUNIFORM_H
#define RAWENGINE_MODELSHADERUNIFORM_H

#endif //RAWENGINE_MODELSHADERUNIFORM_H