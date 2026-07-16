#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camiCube.hpp"

extern "C"{
#include "shaders.h"
#include "textures.h"
}

CamiCubeSystem::CamiCubeSystem(uint capacity) :
    camiCubes_(new CamiCube[capacity]),
    count_(0),
    capacity_(capacity)
{}

CamiCubeSystem::~CamiCubeSystem(void)
{
    delete[] camiCubes_;
}

bool CamiCubeSystem::insert(
    const glm::vec3& position,
    const glm::quat& orientation,
    const float& scale,
    const glm::vec3& rotationAxis,
    const float& rotationRate
)
{
    if (count_ >= capacity_)
    {
        return false;
    }

    camiCubes_[count_].position = position;
    camiCubes_[count_].orientation = orientation;
    camiCubes_[count_].scale = scale;
    camiCubes_[count_].rotationAxis = rotationAxis;
    camiCubes_[count_].rotationRate = rotationRate;

    count_ = count_ + 1;
    return true;
}

void CamiCubeSystem::update(
    const float& secondsDelta
)
{
    secondsElapsed_ = secondsElapsed_ + secondsDelta;
}

uint CamiCubeSystem::getMatrices(
    glm::mat4* start, 
    uint stride,
    uint maxCount
) const
{
    uint i;
    glm::mat4 matrix;
    for (i = 0; i < count_; i++)
    {
        if (i > maxCount)
        {
            return i;
        }

        glm::mat4 matrix = glm::identity<glm::mat4>();
        matrix = glm::translate(matrix, camiCubes_[i].position);
        matrix = glm::rotate(
            matrix, 
            camiCubes_[i].rotationRate * secondsElapsed_,
            camiCubes_[i].rotationAxis
        );
        matrix = matrix * glm::mat4_cast(camiCubes_[i].orientation);
        matrix = glm::scale(matrix, glm::vec3(camiCubes_[i].scale));
        *(start + stride * i) = matrix;
    }

    return i;
}

CamiCubeRenderer::CamiCubeRenderer(
    uint instanceCount
) :
    instanceCount_(instanceCount)
{

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    glGenBuffers(1, &vboVertices_);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
    float vertices[] =
    {
        -1.0f, -1.0f, -1.0f,    0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,    1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,    1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,    2.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,    3.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,   -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,    0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,    1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,    2.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,    3.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,    0.0f,  2.0f,
         1.0f,  1.0f, -1.0f,    1.0f,  2.0f
    };
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glGenBuffers(1, &vboInstances_);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstances_);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        16 * sizeof(GL_FLOAT),
        (void*)0
    );
    glVertexAttribDivisor(2, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    uint cubeElements[] = {
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
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(cubeElements),
        cubeElements,
        GL_STATIC_DRAW
    );

    glBindVertexArray(0);

    shader_ = compileShader(
        &shaderVertexSource, 1,
        &shaderFragmentSource, 1
    );

    glGenTextures(1, &texture_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_BGRA, 
        camiTextureWidth, 
        camiTextureHeight, 
        0, 
        GL_BGRA, 
        GL_UNSIGNED_BYTE, 
        camiTextureBytes
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void CamiCubeRenderer::setInstanceMatrices(
    uint count,
    glm::mat4* data
)
{
    GLuint error;
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstances_);
    error = glGetError();
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(glm::mat4) * count,
        data,
        GL_STATIC_DRAW
    );
    error = glGetError();
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    error = glGetError();
    glVertexAttribPointer(
        2,
        4,
        GL_FLOAT,
        GL_FALSE,
        16 * sizeof(float),
        (void*)0
    );
    error = glGetError();
    glVertexAttribPointer(
        3,
        4,
        GL_FLOAT,
        GL_FALSE,
        16 * sizeof(float),
        (void*)16
    );
    error = glGetError();
    glVertexAttribPointer(
        4,
        4,
        GL_FLOAT,
        GL_FALSE,
        16 * sizeof(float),
        (void*)32
    );
    error = glGetError();
    glVertexAttribPointer(
        5,
        4,
        GL_FLOAT,
        GL_FALSE,
        16 * sizeof(float),
        (void*)48
    );
    error = glGetError();
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    error = glGetError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    error = glGetError();
    return;
}

void CamiCubeRenderer::draw(
    const glm::mat4& matrixProjView
)
{
    GLuint error = glGetError();
    glUseProgram(shader_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glUniform1i(1, 0);
    glUniformMatrix4fv(
        0,
        1,
        GL_FALSE,
        glm::value_ptr(matrixProjView)
    );
    glBindVertexArray(vao_);

    glDrawElementsInstanced(
        GL_TRIANGLES,
        36,
        GL_UNSIGNED_INT,
        (void*)0,
        instanceCount_
    );
    error = glGetError();

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}