#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camiCube.hpp"

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