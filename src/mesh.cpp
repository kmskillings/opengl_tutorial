#include "gl_includes.h"
#include "mesh.hpp"

namespace GlWorld {

Mesh::Mesh(const std::vector<float> &vertexArray)
{
    this->vertexArray = std::vector(vertexArray);
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertexArray.size() * sizeof(float), this->vertexArray.data(), GL_STATIC_DRAW);
}

Mesh::~Mesh(void)
{

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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Mesh* Mesh::Triangle(float width, float height)
{
    std::vector<float> vertexArray = {
        -width/2,   -height/2,
         width/2,   -height/2,
         0.0f,       height/2
    };
    Mesh* mesh = new Mesh(vertexArray);

    return mesh;
}

}