#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camiCubeSystem.hpp"

#include "fixedPackedArray.hpp"
#include "randomGeneration.hpp"
#include "camiCubeOrientation.hpp"

void CamiCubeSystem::init(int seed)
{
    rg_.init(seed);
}

uint32_t CamiCubeSystem::populateCamiCubes(
    uint32_t count,
    float cloudRadius,
    float rotationRateMax,
    FixedPackedArray<glm::vec3>& positions,
    FixedPackedArray<CamiCubeOrientation>& orientations
)
{
    int i;
    for (
        i = 0; 
        i < count && i < positions.capacity && i < orientations.capacity;
        i++
    )
    {
        positions.push(rg_.getUnitBall() * cloudRadius);
        orientations.push(CamiCubeOrientation());
        orientations[i].orientationAngle = rg_.getPositiveFloat() * 2.0f * M_PI;
        orientations[i].orientationAxis = rg_.getUnitSphere();
        orientations[i].rotationRate = rg_.getPositiveFloat() * rotationRateMax;
        orientations[i].rotationAxis = rg_.getUnitSphere();
    }
    return i;
}