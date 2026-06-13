#include "gl_includes.h"
#include "mesh.hpp"

namespace GlWorld {

Mesh::Mesh(void)
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexArray), this->vertexArray, GL_STATIC_DRAW);
}

void Mesh::bindVertexPositionScreenSpace(GLuint index)
{
    this->vertexPositionScreenSpaceIndex = index;
    glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::activate(void)
{
    glEnableVertexArrayAttrib(this->vao, this->vertexPositionScreenSpaceIndex);
}

void Mesh::deactivate(void)
{
    glDisableVertexArrayAttrib(this->vao, this->vertexPositionScreenSpaceIndex);
}

void Mesh::draw(void)
{
    glDrawArrays(GL_TRIANGLES, 0, sizeof(this->vertexArray) / sizeof(this->vertexArray[0]));
}

}