#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include "mesh.hpp"
#include "shaders.h"

namespace GlWorld {

class Material {
public:
    Material(void) {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        this->shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glBindFragDataLocation(this->shaderProgram, 0, "outColor");
        glLinkProgram(this->shaderProgram);
    }
    void activate(void) {
        glUseProgram(this->shaderProgram);
    }
    void deactivate(void) {

    }
    void bindVertexAttributes(Mesh* mesh) {
        GLuint indexVertexPositionScreenSpace = glGetAttribLocation(this->shaderProgram, "position");
        mesh->bindVertexPositionScreenSpace(indexVertexPositionScreenSpace);
    }
private:
    GLuint shaderProgram;
};

}

#endif