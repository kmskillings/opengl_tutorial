#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camiCubeVao.hpp"

constexpr uint locationVertexPosition           = 0;
constexpr uint locationTextureCoords            = 1;
constexpr uint locationModelScale               = 2;
constexpr uint locationModelOrientationAngle    = 3;
constexpr uint locationModelOrienationAxis      = 4;
constexpr uint locationModelRotationRate        = 5;
constexpr uint locationModelRotationAxis        = 6;
constexpr uint locationModelPosition            = 7;
constexpr uint locationColor                    = 8;

CamiCubeVao::CamiCubeVao(
    uint instanceCount,
    glm::vec3* instancePositions,
    CamiCubeOrientation* instanceOrientations
) :
    instanceCount_(instanceCount)
{

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vboVertices_);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
    float vertices[] = {
        -1.0f, -1.0f, -1.0f,    0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,    1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,    1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,    2.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,    3.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,   -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,    0.0f, -1.0f,
         1.0f,  1.0f,  1.0f,    1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,    2.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    3.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    0.0f, -2.0f,
         1.0f,  1.0f, -1.0f,    1.0f, -2.0f
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
    CamiCubeInstance* instanceData = new CamiCubeInstance[instanceCount_];
    for (int i = 0; i < instanceCount_; i++)
    {
        instanceData[i].scale = 0.5f;
        instanceData[i].orientationAngle = instanceOrientations[i].orientationAngle;
        instanceData[i].orientationAxis = instanceOrientations[i].orientationAxis;
        instanceData[i].rotationRate = instanceOrientations[i].rotationRate;
        instanceData[i].rotationAxis = instanceOrientations[i].rotationAxis;
        instanceData[i].modelPosition = instancePositions[i];
        instanceData[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(CamiCubeInstance) * instanceCount_,
        instanceData,
        GL_DYNAMIC_DRAW
    );
    delete instanceData;
    glEnableVertexAttribArray(locationModelScale);
    glVertexAttribPointer(
        locationModelScale,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, scale)
    );
    glVertexAttribDivisor(locationModelScale, 1);
    glEnableVertexAttribArray(locationModelOrientationAngle);
    glVertexAttribPointer(
        locationModelOrientationAngle,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, orientationAngle)
    );
    glVertexAttribDivisor(locationModelOrientationAngle, 1);
    glEnableVertexAttribArray(locationModelOrienationAxis);
    glVertexAttribPointer(
        locationModelOrienationAxis,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, orientationAxis)
    );
    glVertexAttribDivisor(locationModelOrienationAxis, 1);
    glEnableVertexAttribArray(locationModelRotationRate);
    glVertexAttribPointer(
        locationModelRotationRate,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, rotationRate)
    );
    glVertexAttribDivisor(locationModelRotationRate, 1);
    glEnableVertexAttribArray(locationModelRotationAxis);
    glVertexAttribPointer(
        locationModelRotationAxis,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, rotationAxis)
    );
    glVertexAttribDivisor(locationModelRotationAxis, 1);
    glEnableVertexAttribArray(locationModelPosition);
    glVertexAttribPointer(
        locationModelPosition,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, modelPosition)
    );
    glVertexAttribDivisor(locationModelPosition, 1);
    glEnableVertexAttribArray(locationColor);
    glVertexAttribPointer(
        locationColor,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CamiCubeInstance),
        (void*)offsetof(CamiCubeInstance, color)
    );
    glVertexAttribDivisor(locationColor, 1);

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
    triangleCount_ = sizeof(cubeElements) / sizeof(cubeElements[0]) / 3;

    glBindVertexArray(0);
}

CamiCubeVao::~CamiCubeVao(void)
{
    glDeleteBuffers(1, &vboInstances_);
    glDeleteBuffers(1, &vboVertices_);
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);
}

GLuint CamiCubeVao::getVao(void) const
{
    return vao_;
}

GLuint CamiCubeVao::getEbo(void) const
{
    return ebo_;
}

GLuint CamiCubeVao::getInstanceVbo(void) const
{
    return vboInstances_;
}

GLuint CamiCubeVao::getTriangleCount(void) const
{
    return triangleCount_;
}

GLuint CamiCubeVao::getInstanceCount(void) const
{
    return instanceCount_;
}