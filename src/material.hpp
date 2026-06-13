#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include "gl_includes.h"

#include "mesh.hpp"
#include "shaders.h"

namespace GlWorld {

class Material {
public:
    Material(glm::vec4 color) {
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

        this->color = color;
        this->uniformColor = glGetUniformLocation(this->shaderProgram, "color");
    }
    void activate(void) {
        glUseProgram(this->shaderProgram);
        glUniform4fv(this->uniformColor, 1, glm::value_ptr(this->color));
    }
    void deactivate(void) {

    }
    void bindVertexAttributes(Mesh* mesh) {
        GLuint indexVertexPositionScreenSpace = glGetAttribLocation(this->shaderProgram, "position");
        mesh->bindVertexPositionScreenSpace(indexVertexPositionScreenSpace);
    }
    void setColor(glm::vec4 color) {
        this->color = color;
    }
private:
    GLuint shaderProgram;
    GLuint uniformColor;
    glm::vec4 color;
};

}

#endif