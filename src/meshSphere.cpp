#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "meshSphere.hpp"

constexpr uint vertexFloats = 5; 

MeshSphere::MeshSphere(
    uint segmentsVertical,
    uint segmentsHorizontal,
    float radius
)
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    uint vertexCount = (segmentsVertical + 1) * (segmentsHorizontal + 1);
    float* vertices = new float[vertexCount * vertexFloats];
    for (int i = 0; i < segmentsVertical + 1; i++)
    {
        float elevation = M_PI * i / segmentsVertical;
        float y = radius * cos(elevation);
        float r = radius * sin(elevation);
        for (int j = 0; j < segmentsHorizontal + 1; j++)
        {
            float azimuth = 2 * M_PI * j / segmentsHorizontal;
            float x = -r * sin(azimuth);
            float z = -r * cos(azimuth);
            uint vertexIndex = (segmentsHorizontal + 1) * i + j; 
            vertices[vertexFloats * vertexIndex + 0] = x;
            vertices[vertexFloats * vertexIndex + 1] = y;
            vertices[vertexFloats * vertexIndex + 2] = z;
        }
    }
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexFloats * vertexCount * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        vertexFloats * sizeof(float),
        (void*)0
    );
    glBindBuffer(GL_VERTEX_ARRAY, 0);

    glGenBuffers(1, &eboLines_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboLines_);
    uint lineCount = 
        (segmentsHorizontal + 1) * segmentsVertical +
        segmentsHorizontal * (segmentsVertical + 1)
    ;
    countLines_ = lineCount;
    uint* lines = new uint[2 * lineCount];
    // Vertical lines
    for (int i = 0; i < segmentsVertical; i++)
    {
        for (int j = 0; j < segmentsHorizontal + 1; j++)
        {
            uint lineIndex = (segmentsHorizontal + 1) * i + j;
            uint vertexAIndex = (segmentsHorizontal + 1) * (i + 0) + j;
            uint vertexBIndex = (segmentsHorizontal + 1) * (i + 1) + j;
            lines[2 * lineIndex + 0] = vertexAIndex;
            lines[2 * lineIndex + 1] = vertexBIndex;
        }
    }
    // Horizontal lines
    for (int i = 0; i < segmentsVertical + 1; i++)
    {
        for (int j = 0; j < segmentsHorizontal; j++)
        {
            uint lineIndex = 
                (segmentsHorizontal + 1) * segmentsVertical +
                (segmentsHorizontal) * i + j
            ;
            uint vertexAIndex = (segmentsHorizontal + 1) * i + j;
            uint vertexBIndex = (segmentsHorizontal + 1) * i + j + 1;
            lines[2 * lineIndex + 0] = vertexAIndex;
            lines[2 * lineIndex + 1] = vertexBIndex;
        }
    }
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        2 * lineCount * sizeof(uint),
        lines,
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &eboTriangles_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboTriangles_);
    uint quadCount = segmentsVertical * segmentsHorizontal;
    countTriangles_ = 2 * quadCount;
    uint* triangles = new uint[6 * quadCount];
    for (int i = 0; i < segmentsVertical; i++)
    {
        for (int j = 0; j < segmentsHorizontal; j++)
        {
            uint quadIndex = segmentsHorizontal * i + j;
            triangles[6 * quadIndex + 0] = (segmentsHorizontal + 1) * (i + 0) + (j + 0);
            triangles[6 * quadIndex + 1] = (segmentsHorizontal + 1) * (i + 1) + (j + 0);
            triangles[6 * quadIndex + 2] = (segmentsHorizontal + 1) * (i + 1) + (j + 1);
            triangles[6 * quadIndex + 3] = (segmentsHorizontal + 1) * (i + 1) + (j + 1);
            triangles[6 * quadIndex + 4] = (segmentsHorizontal + 1) * (i + 0) + (j + 1);
            triangles[6 * quadIndex + 5] = (segmentsHorizontal + 1) * (i + 0) + (j + 0);
        }
    }
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        6 * quadCount * sizeof(uint),
        triangles,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    delete vertices;
    delete lines;
    delete triangles;
}

GLuint MeshSphere::getVao(void) const
{
    return vao_;
}

GLuint MeshSphere::getEboTriangles(void) const
{
    return eboTriangles_;
}

uint MeshSphere::getCountTriangles(void) const
{
    return countTriangles_;
}

GLuint MeshSphere::getEboLines(void) const
{
    return eboLines_;
}

uint MeshSphere::getCountLines(void) const
{
    return countLines_;
}