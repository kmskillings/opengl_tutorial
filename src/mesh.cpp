#include "gl_includes.h"

#include "mesh.hpp"

#include <memory>
#include <vector>

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

bool MeshTextured::has4dPosition(void) const
{
    return true;
}

void MeshTextured::bind4dPosition(GLuint location)
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

bool MeshTextured::has2dTextureCoords(void) const
{
    return true;
}

void MeshTextured::bind2dTextureCoords(GLuint location)
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

bool MeshTextured::hasVertexNormals(void) const
{
    return false;
}

void MeshTextured::bindVertexNormals(GLuint location)
{
    return;
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

std::shared_ptr<MeshTextured> MeshTextured::sphere(
    float radius, 
    unsigned int segmentsVertical, 
    unsigned int segmentsHorizontal
)
{
    std::vector<float> vertices;
    std::vector<GLuint> elements;

    // i is the vertical iterator. j is the horizontal iterator.
    for (int i = 0; i < segmentsVertical + 1; i++)
    {
        float elevation = M_PI / segmentsVertical * i;
        float r = radius * sin(elevation);
        float y = radius * cos(elevation);
        float v = 1.0f - ((float)i / (float)segmentsVertical);
        for (int j = 0; j < segmentsHorizontal + 1; j++)
        {
            float azimuth = 2 * M_PI / segmentsHorizontal * j;
            float x = r * sin(azimuth);
            float z = r * cos(azimuth);
            float u = (float)j / (float)segmentsHorizontal;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(1.0f);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    for (int i = 0; i < segmentsVertical; i++)
    {
        for (int j = 0; j < segmentsHorizontal; j++)
        {
            GLuint lowerLeft =  (segmentsHorizontal + 1) * (i + 1) + j    ;
            GLuint lowerRight = (segmentsHorizontal + 1) * (i + 1) + j + 1;
            GLuint upperLeft =  (segmentsHorizontal + 1) *  i      + j    ;
            GLuint upperRight = (segmentsHorizontal + 1) *  i      + j + 1;

            elements.push_back(lowerLeft);
            elements.push_back(upperRight);
            elements.push_back(upperLeft);
            elements.push_back(upperRight);
            elements.push_back(lowerLeft);
            elements.push_back(lowerRight);
        }
    }

    return std::make_shared<MeshTextured>(vertices, elements);
}

}