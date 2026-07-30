#ifndef _CAMI_CUBE_SYSTEM_HPP
#define _CAMI_CUBE_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "randomGeneration.hpp"

template<typename T>
struct FixedPackedArray;

struct CamiCubeOrientation;

// Cami Cube System
//
// The Cami Cube System is responsible for creating the Cami Cubes during
// startup. Because the rotation of the Cami Cubes is calculated as-needed
// from the global time and rotation parameters, the Cami Cube System is not
// required to perform any updates.

class CamiCubeSystem
{

private:

    RandomGenerator rg_;

public:

    void init(int seed);

    // Creates the given number of Cami Cubes and places the position and
    // orientation components inside the given arrays. Returns the actual
    // number of Cami Cubes created, which will be less than requested if
    // the arrays are too small.
    //
    // This function does not consider whether the arrays are chunked. Any
    // chunking of the arrays once they have been populated is someone else's
    // responsibility.
    uint32_t populateCamiCubes(
        uint32_t count,
        float cloudRadius,
        float rotationRateMax,
        FixedPackedArray<glm::vec3>& positions,
        FixedPackedArray<CamiCubeOrientation>& orientations
    );

};

#endif