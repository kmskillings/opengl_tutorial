#ifndef _COLLISION_MEDIUM_SYSTEM_HPP
#define _COLLISION_MEDIUM_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T> struct FixedPackedArray;
struct Collision;

class CollisionMediumSystem
{

private:

public:

void detectCollisions(
    float cubeRadius,
    float sphereRadius,
    const glm::vec3& spherePosition,
    const FixedPackedArray<glm::vec3> camiCubePositions,
    const FixedPackedArray<Collision>& broadPhaseCollisions,
    FixedPackedArray<Collision>& mediumPhaseCollisions
) const;

};

#endif
