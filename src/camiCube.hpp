#ifndef _CAMI_CUBE_HPP
#define _CAMI_CUBE_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Cami Cubes are simple cubes textured with Cami's face. They float in 3D
// space and rotate around a fixed axis at a fixed rate.
// Because the cubes rotate at a fixed rate, a global time counter can be used
// to track the rotation of all cubes. Then all Cami Cube data is read-only and
// used only to create the model matrix.

class CamiCubeSystem
{

private:

    struct CamiCube
    {
        glm::vec3 position;
        glm::quat orientation;
        float scale;
        glm::vec3 rotationAxis;
        float rotationRate;
    };

    CamiCube* camiCubes_;
    uint count_;
    uint capacity_;

    float secondsElapsed_;

public:

    CamiCubeSystem(uint capacity) :
        camiCubes_(new CamiCube[capacity]),
        count_(0),
        capacity_(capacity)
    {}

    ~CamiCubeSystem(void)
    {
        delete[] camiCubes_;
    }

    // Insert a new CamiCube with the given parameters. Returns whether it was
    // successfull inserted.
    bool insert(
        const glm::vec3& position,
        const glm::quat& orientation,
        const float& scale,
        const glm::vec3& rotationAxis,
        const float& rotationRate
    )
    {
        if (count_ == capacity_)
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

    // Updates all CamiCubes in the system.
    void update(
        const float& secondsDelta
    )
    {
        secondsElapsed_ = secondsElapsed_ + secondsDelta;
    }

    // Gets the model matrices of the CamiCubes in the system. Writes them
    // to an array starting at the "start" pointer. The stride is the number
    // of mat4-sized blocks to skip between writes (1 to have the array be
    // packed). End points to the first null element of the array. If this or
    // any subsequent addresses would be written, no more are written and the
    // function returns. The total number of matrices written is always
    // returned.
    uint getMatrices(
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

};

#endif