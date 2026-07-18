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

constexpr uint locationVertexPosition           = 0;
constexpr uint locationTextureCoords            = 1;
constexpr uint locationModelScale               = 2;
constexpr uint locationModelOrientationAngle    = 3;
constexpr uint locationModelOrienationAxis      = 4;
constexpr uint locationModelRotationRate        = 5;
constexpr uint locationModelRotationAxis        = 6;
constexpr uint locationModelPosition            = 7;

CamiCubeSystem::CamiCubeSystem(uint capacity) :
    camiCubes_(new CamiCube[capacity]),
    count_(0),
    capacity_(capacity),
    secondsElapsed_(0),
    matrixProjView_(glm::mat4(0.0f)),
    instancesDirty_(false),
    matrixDirty_(false)
{

    // Configure the VAO
    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Vertex array
    glGenBuffers(1, &vboVertices_);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
    float vertices[] = {
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
    glEnableVertexAttribArray(locationVertexPosition);
    glVertexAttribPointer(
        locationVertexPosition,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(vertices[0]),
        (void*)(sizeof(float) * 0)
    );
    glEnableVertexAttribArray(locationTextureCoords);
    glVertexAttribPointer(
        locationTextureCoords,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(vertices[0]),
        (float*)(sizeof(float) * 3)
    );

    // Instance buffer
    glGenBuffers(1, &vboInstances_);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstances_);
    glEnableVertexAttribArray(locationModelScale);
    glVertexAttribPointer(
        locationModelScale,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCube),
        (void*)offsetof(CamiCube, scale)
    );
    glVertexAttribDivisor(locationModelScale, 1);
    glEnableVertexAttribArray(locationModelOrientationAngle);
    glVertexAttribPointer(
        locationModelOrientationAngle,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCube),
        (void*)offsetof(CamiCube, orientationAngle)
    );
    glVertexAttribDivisor(locationModelOrientationAngle, 1);
    glEnableVertexAttribArray(locationModelOrienationAxis);
    glVertexAttribPointer(
        locationModelOrienationAxis,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCube),
        (void*)offsetof(CamiCube, orientationAxis)
    );
    glVertexAttribDivisor(locationModelOrienationAxis, 1);
    glEnableVertexAttribArray(locationModelRotationRate);
    glVertexAttribPointer(
        locationModelRotationRate,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCube),
        (void*)offsetof(CamiCube, rotationRate)
    );
    glVertexAttribDivisor(locationModelRotationRate, 1);
    glEnableVertexAttribArray(locationModelRotationAxis);
    glVertexAttribPointer(
        locationModelRotationAxis,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCube),
        (void*)offsetof(CamiCube, rotationAxis)
    );
    glVertexAttribDivisor(locationModelRotationAxis, 1);
    glEnableVertexAttribArray(locationModelPosition);
    glVertexAttribPointer(
        locationModelPosition,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCube),
        (void*)offsetof(CamiCube, modelPosition)
    );
    glVertexAttribDivisor(locationModelPosition, 1);

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

    // Set up shaders and textures
    shader_ = compileShader(
        &camiCubeVertexSource, 1,
        &camiCubeFragmentSource, 1
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

CamiCubeSystem::~CamiCubeSystem(void)
{
    delete[] camiCubes_;
}

bool CamiCubeSystem::insert(
    const float&        scale,
    const float&        orientationAngle,
    const glm::vec3&    orientationAxis,
    const float&        rotationRate,
    const glm::vec3&    rotationAxis,
    const glm::vec3&    position
)
{
    if (count_ >= capacity_)
    {
        return false;
    }

    camiCubes_[count_] = {
        scale,
        orientationAngle,
        orientationAxis,
        rotationRate,
        rotationAxis,
        position
    };

    count_ = count_ + 1;

    instancesDirty_ = true;

    return true;
}

void CamiCubeSystem::setMatrixProjView(
    const glm::mat4& matrix
)
{
    matrixProjView_ = matrix;
}

void CamiCubeSystem::update(
    const float& secondsDelta
)
{   
    secondsElapsed_ = secondsElapsed_ + secondsDelta;
}

void CamiCubeSystem::draw(void)
{
    GLuint error = glGetError();
    // Prepare and update shader / material
    glUseProgram(shader_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glUniform1i(2, 0);
    glUniformMatrix4fv(
        0,
        1,
        GL_FALSE,
        glm::value_ptr(matrixProjView_)
    );
    glUniform1f(
        1,
        secondsElapsed_
    );

    // Prepare and update vao
    glBindVertexArray(vao_);
    if (instancesDirty_)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vboInstances_);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(CamiCube) * count_,
            camiCubes_,
            GL_DYNAMIC_DRAW
        );
        instancesDirty_ = false;
    }

    error = glGetError();

    glDrawElementsInstanced(
        GL_TRIANGLES,
        36,
        GL_UNSIGNED_INT,
        (void*)0,
        count_
    );

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}