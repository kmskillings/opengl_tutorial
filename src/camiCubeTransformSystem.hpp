#ifndef _CAMI_CUBE_ORIENTATION_SYSTEM
#define _CAMI_CUBE_ORIENTATION_SYSTEM

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
struct FixedPackedArray;

struct Collision;

struct CamiCubeOrientation;

class CamiCubeTransformSystem
{

private:

public:

void calculateTransforms(
    float secondsElapsed,
    const FixedPackedArray<Collision>& collisions,
    const FixedPackedArray<glm::vec3>& positions,
    const FixedPackedArray<CamiCubeOrientation>& orientations,
    FixedPackedArray<glm::mat4>& transforms
) const;

};

#endif