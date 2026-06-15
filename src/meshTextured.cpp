#include "meshTextured.hpp"

namespace GlWorld
{

MeshTextured::MeshTextured(
    const std::vector<float> &vertices,
    const std::vector<GLuint> &elements
)
{
    this->vertices = std::vector(vertices);
    this->elements = std::vector(elements);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        this->vertices.size() * sizeof(float),
        this->vertices.data(),
        GL_STATIC_DRAW
    );
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        this->elements.size() * sizeof(GLuint),
        this->elements.data(),
        GL_STATIC_DRAW
    );
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void MeshTextured::bindPosition(GLuint location)
{
    this->locationPosition = location;
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(
        location, 
        4, 
        GL_FLOAT, 
        GL_FALSE, 
        6*sizeof(float), 
        0
    );
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshTextured::bindTextureCoords(GLuint location)
{
    this->locationTextureCoords = location;
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(
        location, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        6*sizeof(float), 
        (void*)(4*sizeof(float))
    );
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshTextured::activate(void)
{
    glBindVertexArray(this->vao);
    glEnableVertexArrayAttrib(this->vao, this->locationPosition);
    glEnableVertexArrayAttrib(this->vao, this->locationTextureCoords);
}

void MeshTextured::deactivate(void)
{
    glDisableVertexArrayAttrib(this->vao, this->locationPosition);
    glDisableVertexArrayAttrib(this->vao, this->locationTextureCoords);
    glBindVertexArray(0);
}

void MeshTextured::draw(void)
{
    GLuint error = glGetError();
    glDrawElements(
        GL_TRIANGLES, 
        this->elements.size(), 
        GL_UNSIGNED_INT, 0)
    ;
}

std::shared_ptr<MeshTextured> MeshTextured::cube(float sideLength)
{
    float s = sideLength / 2;
    std::vector<float> vertices = {
        // XYZW coords                  Texture coords
        -s, -s, -s,  1.0f,  0.0f, -1.0f,
         s, -s, -s,  1.0f,  1.0f, -1.0f,
        -s, -s, -s,  1.0f, -1.0f,  0.0f,
        -s, -s,  s,  1.0f,  0.0f,  0.0f,
         s, -s,  s,  1.0f,  1.0f,  0.0f,
         s, -s, -s,  1.0f,  2.0f,  0.0f,
        -s, -s, -s,  1.0f,  3.0f,  0.0f,
        -s,  s, -s,  1.0f, -1.0f,  1.0f,
        -s,  s,  s,  1.0f,  0.0f,  1.0f,
         s,  s,  s,  1.0f,  1.0f,  1.0f,
         s,  s, -s,  1.0f,  2.0f,  1.0f,
        -s,  s, -s,  1.0f,  3.0f,  1.0f,
        -s,  s, -s,  1.0f,  0.0f,  2.0f,
         s,  s, -s,  1.0f,  1.0f,  2.0f
    };
    std::vector<GLuint> elements = {
        0, 1, 4,
        4, 3, 0,
        2, 3, 8,
        8, 7, 2,
        3, 4, 9,
        9, 8, 3,
        4, 5, 10,
        10, 9, 4,
        5, 6, 11,
        11, 10, 5,
        8, 9, 13,
        13, 12, 8
    };
    return std::make_unique<MeshTextured>(vertices, elements);
}

}